#include "tvm/c_api.h"
#include "c_api_common.h"
#include "c_api_registry.h"

struct TVMAPIThreadLocalEntry {
    std::string last_error;
    std::vector<std::string> ret_vec_str;
    std::vector<const char *> ret_vec_charp;
    std::vector<tvm::APIVariantValue> arg_stack;
    tvm::APIVariantValue ret_value;
    inline void Clear() {
        arg_stack.clear();
        ret_value.sptr.reset();
    }
    inline void SetReturn(ArgVariant* ret_val, int* ret_typeid);
};

namespace tvm {

inline std::string Type2String(const Type& t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

inline Type String2Type(std::string s) {
    std::istringstream is(s);
    halide_type_code_t code;
    if (s.substr(0, 3) == "int") {
        code = Type::Int; s = s.substr(3);
    } else if (s.substr(0, 4) == "uint") {
        code = Type::UInt; s = s.substr(4);
    } else if (s.substr(0, 5) == "float") {
        code = Type::Float; s =s.substr(5);
    } else if (s.substr(0, 5) == "float") {
        code = Type::Float; s = s.substr(5);
    } else {
        LOG(FATAL) << "unknow type " << s;
    }

    int bits, lanes = 0;
    if (sscanf(s.c_str(), "%dx%d", &bits, &lanes) == 0) {
        LOG(FATAL) << "unknow type " << s;
    }
    return Type(code, bits, lanes);
}

}; // end of namespace tvm

using namespace tvm;

typedef dmlc::ThreadLocalStore<TVMAPIThreadLocalEntry> TVMAPIThreadLocalStore;

using TVMAPINode = std::shared_ptr<Node>;

struct APIAttrGetter : public AttrVisitor {
    std::string skey;
    APIVariantValue* ret;

    void Visit(const char* key, double* value) final {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, int64_t* value) final {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, uint64_t* value) final {
        CHECK_LE(value[0], std::numeric_limits<int64_t>::max())
            << "cannot return too big constant";
        if (skey == key) *ret = static_cast<int64_t>(value[0]);
    }
    void Visit(const char* key, int* value) final {
        if (skey == key) *ret = static_cast<int64_t>(value[0]);
    }
    void Visit(const char* key, bool* value) final {
        if (skey == key) *ret = static_cast<int64_t>(value[0]);
    }
    void Visit(const char* key, Type* value) final {
        if (skey == key) *ret = Type2String(value[0]);
    }
    void Visit(const char* key, std::string* value) final {
        if (skey == key) *ret = value[0];
    }
    void Visit(const char* key, NodeRef* value) final {
        if (skey == key) *ret = value[0];
    }
};

struct APIAttrDir : public AttrVisitor {
    std::vector<std::string>* names;

    void Visit(const char* key, double* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, int64_t* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, uint64_t* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, bool* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, int* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, Type* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, std::string* value) final {
        names->push_back(key);
    }
    void Visit(const char* key, NodeRef* value) final {
        names->push_back(key);
    }
};

const char *TVMGetLastError() {
    return TVMAPIThreadLocalStore::Get()->last_error.c_str();
}

void TVMAPISetLastError(const char* msg) {
    TVMAPIThreadLocalStore::Get()->last_error = msg;
}

int TVMListFunctionNames(int *out_size, const char*** out_array) {
    API_BEGIN();
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();
    ret->ret_vec_str = dmlc::Registry<APIFunctionReg>::ListAllNames();
    ret->ret_vec_charp.clear();
    for (size_t i = 0; i < ret->ret_vec_str.size(); ++i) {
        ret->ret_vec_charp.push_back(ret->ret_vec_str[i].c_str());
    }
    *out_array = dmlc::BeginPtr(ret->ret_vec_charp);
    *out_size = static_cast<int>(ret->ret_vec_str.size());
    API_END();
}

int TVMGetFunctionHandle(const char* fname, FunctionHandle* out) {
    API_BEGIN();
    const APIFunctionReg* reg = dmlc::Registry<APIFunctionReg>::Find(fname);
    CHECK(reg != nullptr) << "cannot find function " << fname;
    *out = (FunctionHandle)reg;
    API_END();
}

int TVMGetFunctionInfo(FunctionHandle handle,
                  const char **real_name,
                  const char **description,
                  int *num_doc_args,
                  const char ***arg_names,
                  const char ***arg_type_infos,
                  const char ***arg_descriptions,
                  const char **return_type) {
    const auto *op = static_cast<const APIFunctionReg *>(handle);
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();

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
    *arg_descriptions = dmlc::BeginPtr(ret->ret_vec_charp) + (op->arguments.size() * 2);
    API_END();
}


int TVMPushStack(ArgVariant arg, int type_id) {
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->arg_stack.resize(ret->arg_stack.size() + 1);
    APIVariantValue& v = ret->arg_stack.back();
    v.type_id = static_cast<ArgVariantID>(type_id);
    if (type_id == kStr) {
        v = arg.v_str;
    } else if (type_id == kNodeHandle) {
        v.sptr = *static_cast<TVMAPINode*>(arg.v_handle);
    } else {
        v.v_union = arg;
    }
    API_END_HANDLE_ERROR(ret->Clear());
}

int TVMFunctionCall(FunctionHandle handle, 
                    ArgVariant* ret_val,
                    int* ret_typeid) {
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();
    API_BEGIN();
    const auto *op = static_cast<const APIFunctionReg *>(handle);
    op->body(ret->arg_stack, &(ret->ret_value));
    ret->SetReturn(ret_val, ret_typeid);
    ret->arg_stack.clear();
    API_END_HANDLE_ERROR(ret->Clear());
}

int TVMNodeFree(NodeHandle handle) {
    API_BEGIN();
    delete static_cast<TVMAPINode*>(handle);
    API_END();
}

int TVMNodeGetAttr(NodeHandle handle,
                   const char* key,
                   ArgVariant* ret_val,
                   int* ret_typeid) {
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->ret_value.type_id = kNull;
    APIAttrGetter getter;
    getter.skey = key;
    getter.ret = &(ret->ret_value);
    TVMAPINode* tnode = static_cast<TVMAPINode*>(handle);
    if (getter.skey == "type_key") {
        ret_val->v_str = (*tnode)->type_key();
        *ret_typeid = kStr;
    } else {
        (*tnode)->VisitAttrs(&getter);
        if (ret->ret_value.type_id != kNull) {
            ret->SetReturn(ret_val, ret_typeid);
        } else {
            *ret_typeid = kNull;
        }
    }
    API_END_HANDLE_ERROR(ret->Clear());
}

int TVMNodeListAttrNames(NodeHandle handle,
                         int *out_size,
                         const char*** out_array) {
    TVMAPIThreadLocalEntry *ret = TVMAPIThreadLocalStore::Get();
    API_BEGIN();
    ret->ret_vec_str.clear();
    TVMAPINode* tnode = static_cast<TVMAPINode*>(handle);
    APIAttrDir dir;
    dir.names = &(ret->ret_vec_str);
    (*tnode)->VisitAttrs(&dir);
    ret->ret_vec_charp.clear();
    for (size_t i = 0; i < ret->ret_vec_str.size(); ++i) {
        ret->ret_vec_charp.push_back(ret->ret_vec_str[i].c_str());
    }
    *out_array = dmlc::BeginPtr(ret->ret_vec_charp);
    *out_size = static_cast<int>(ret->ret_vec_str.size());
    API_END();
}

inline void TVMAPIThreadLocalEntry::SetReturn(ArgVariant* ret_val, int* ret_typeid) {
    APIVariantValue& rv = ret_value;
    *ret_typeid = rv.type_id;
    if (rv.type_id == kNodeHandle) {
        if (rv.sptr.get() != nullptr) {
            ret_val->v_handle = new TVMAPINode(std::move(rv.sptr));
        } else {
            ret_val->v_handle = nullptr;
        }
    } else {
        *ret_val = rv.v_union;
    }
}

