#include "txtvm/c_api.h"
#include "txtvm/op.h"
#include "./c_api_common.h"
#include "./c_api_registry.h"

/*! \brief entry to to easily hold returning information */
struct TXTVMAPIThreadLocalEntry {
    /*! \brief hold last error */
    std::string last_error;
    /*! \brief result holder for returning strings */
    std::vector<std::string> ret_vec_str;
    /*! \brief result holder for returning string pointers */
    std::vector<const char *> ret_vec_charp;
    /*! \brief argument stack */
    std::vector<txtvm::APIVariantValue> arg_stack;
    /*! \brief return value */
    txtvm::APIVariantValue ret_value;
    // clear calling stack
    inline void Clear() {
        arg_stack.clear();
        ret_value.sptr.reset();
    }
    inline void SetReturn(ArgVariant* ret_val, int* ret_typeid);
};  // struct end of TXTVMAPIThreadLocalEntry

using namespace txtvm;

/*! \brief Thread local store that can be used to hold return values. */
typedef dmlc::ThreadLocalStore<TXTVMAPIThreadLocalEntry> TXTVMAPIThreadLocalStore;

using TXTVMAPINode = std::shared_ptr<Node>;

struct APIAttrGetter : public AttrVisitor {
    std::string skey;
    APIVariantValue* ret;

    void Visit(const char* key, double* value) override {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, int64_t* value) override {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, DataType* value) override {
        if (skey == key) *ret = static_cast<int64_t>(value[0]);
    }
    void Visit(const char* key, std::string* value) override {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, const UnaryOp** value) override {
        if (skey == key) *ret = value[0]->FunctionName();
    }
    void Visit(const char* key, const BinaryOp** value) override {
        if (skey == key) *ret = value[0]->FunctionName();
    }
};  // struct end of APIAttrGetter

struct APIAttrDir : public AttrVisitor {
    std::vector<std::string>* names;

    void Visit(const char* key, double* value) override {
        names->push_back(key);
    }

    void Visit(const char* key, int64_t* value) override {
        names->push_back(key);
    }

    void Visit(const char* key, DataType* value) override {
        names->push_back(key);
    }

    void Visit(const char* key, std::string* value) override {
        names->push_back(key);
    }

    void Visit(const char* key, const UnaryOp** value) override {
        names->push_back(key);
    }

    void Visit(const char* key, const BinaryOp** value) override {
        names->push_back(key);
    }

}; // struct end of APIAttrDir


const char *TXTVMGetLastError() {
    return TXTVMAPIThreadLocalStore::Get()->last_error.c_str();
}

void TXTVMAPISetLastError(const char* msg) {
    TXTVMAPIThreadLocalStore::Get()->last_error = msg;
}

int TXTVMListFunctionNames(int *out_size,
                            const char*** out_array) {
    API_BEGIN();
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();
    ret->ret_vec_str = dmlc::Registry<APIFunctionReg>::ListAllNames();
    ret->ret_vec_charp.clear();
    for (size_t i = 0; i < ret->ret_vec_str.size(); ++i) {
        ret->ret_vec_charp.push_back(ret->ret_vec_str[i].c_str());
    }
    *out_array = dmlc::BeginPtr(ret->ret_vec_charp);
    *out_size = static_cast<int>(ret->ret_vec_str.size());
    API_END();
}

int TXTVMGetFunctionHandle(const char* fname,
                            FunctionHandle* out) {
    API_BEGIN();
    const APIFunctionReg* reg = dmlc::Registry<APIFunctionReg>::Find(fname);
    CHECK(reg != nullptr) << "cannot find function " << fname;
    *out = (FunctionHandle)reg;
    API_END();
}

int TXTVMGetFunctionInfo(FunctionHandle handle,
                         const char **real_name,
                         const char **description,
                         int *num_doc_args,
                         const char ***arg_names,
                         const char ***arg_type_infos,
                         const char ***arg_descriptions,
                         const char **return_type) {
    const auto *op = static_cast<const APIFunctionReg *>(handle);
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();

    API_BEGIN();
    *real_name = op->name.c_str();
    *description = op->description.c_str();
    *num_doc_args = static_cast<int>(op->arguments.size());
    if (return_type) *return_type = nullptr;
    ret->ret_vec_charp.clear();
    for (size_t i = 0; i < op->arguments.size(); ++i) {
        ret->ret_vec_charp.push_back(op->arguments[i].name.c_str());
    }
    for (size_t i = 0; i < op->arguments.size(); ++i) {
        ret->ret_vec_charp.push_back(op->arguments[i].type_info_str.c_str());
    }
    for (size_t i = 0; i < op->arguments.size(); ++i) {
        ret->ret_vec_charp.push_back(op->arguments[i].description.c_str());
    }
    *arg_names = dmlc::BeginPtr(ret->ret_vec_charp);
    *arg_type_infos = dmlc::BeginPtr(ret->ret_vec_charp) + op->arguments.size();
    *arg_descriptions = dmlc::BeginPtr(ret->ret_vec_charp) + (op->arguments.size() + 2);
    API_END();
}

int TXTVMPushStack(ArgVariant arg, int type_id) {
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->arg_stack.resize(ret->arg_stack.size() + 1);
    APIVariantValue& v = ret->arg_stack.back();
    v.type_id = static_cast<ArgVariantID>(type_id);
    if (type_id == kStr) {
        v = arg.v_str;
    } else if (type_id == kNodeHandle) {
        v.sptr = *static_cast<TXTVMAPINode*>(arg.v_handle);
    } else {
        v.v_union = arg;
    }
    API_END_HANDLE_ERROR(ret->Clear());
}

int TXTVMFunctionCall(FunctionHandle handle,
                        ArgVariant* ret_val,
                        int* ret_typeid) {
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();
    API_BEGIN();
    const auto *op = static_cast<const APIFunctionReg *>(handle);
    op->body(ret->arg_stack, &(ret->ret_value));
    ret->SetReturn(ret_val, ret_typeid);
    ret->arg_stack.clear();
    API_END_HANDLE_ERROR(ret->Clear());
}

int TXTVMNodeFree(NodeHandle handle) {
    API_BEGIN();
    delete static_cast<TXTVMAPINode*>(handle);
    API_END();
}

int TXTVMNodeGetAttr(NodeHandle handle,
                    const char* key,
                    ArgVariant* ret_val,
                    int* ret_typeid) {
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->ret_value.type_id = kNull;
    APIAttrGetter getter;
    getter.skey = key;
    getter.ret = &(ret->ret_value);
    TXTVMAPINode* tnode = static_cast<TXTVMAPINode*>(handle);
    if (getter.skey == "type_key") {
        ret_val->v_str = (*tnode)->type_key();
        *ret_typeid = kStr;
    } else {
        (*tnode)->VisitAttrs(&getter);
        if (ret->ret_value.type_id != kNull) {
            ret->SetReturn(ret_val, ret_typeid);
        } else {
            const std::string& skey = getter.skey;
            (*tnode)->VisitNodeRefFields([&skey, ret](const char* key, NodeRef* ref) {
                if (key == skey) ret->ret_value = *ref;
            }); 
            if (ret->ret_value.type_id != kNull) {
                ret->SetReturn(ret_val, ret_typeid);
            } else {
                *ret_typeid = kNull;
            }
        }
    }
    API_END_HANDLE_ERROR(ret->Clear());
}

int TXTVMNodeListAttrNames(NodeHandle handle,
                           int *out_size,
                           const char*** out_array) {
    TXTVMAPIThreadLocalEntry *ret = TXTVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->ret_vec_str.clear();
    TXTVMAPINode* tnode = static_cast<TXTVMAPINode*>(handle);
    APIAttrDir dir;
    dir.names = &(ret->ret_vec_str);
    (*tnode)->VisitAttrs(&dir);
    (*tnode)->VisitNodeRefFields([ret](const char* key, NodeRef* ref){
                ret->ret_vec_str.push_back(key);
            });
    ret->ret_vec_charp.clear();
    for (size_t i = 0; i < ret->ret_vec_str.size(); ++i) {
        ret->ret_vec_charp.push_back(ret->ret_vec_str[i].c_str());
    }
    *out_array = dmlc::BeginPtr(ret->ret_vec_charp);
    *out_size = static_cast<int>(ret->ret_vec_str.size());
    API_END();
}


inline void TXTVMAPIThreadLocalEntry::SetReturn(ArgVariant* ret_val,
                                                int* ret_typeid) {
    APIVariantValue& rv = ret_value;
    *ret_typeid = rv.type_id;
    if (rv.type_id == kNodeHandle) {
        if (rv.sptr.get() != nullptr) {
            ret_val->v_handle = new TXTVMAPINode(std::move(rv.sptr));
        } else {
            ret_val->v_handle = nullptr;
        }
    } else {
        *ret_val = rv.v_union;
    }
}
