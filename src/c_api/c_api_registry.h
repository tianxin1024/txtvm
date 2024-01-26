#ifndef TVM_C_API_C_API_REGISTRY_H_
#define TVM_C_API_C_API_REGISTRY_H_

#include "tvm/base.h"
#include "tvm/expr.h"
#include "tvm/c_api.h"
#include <memory>
#include <limits>
#include <string>
#include <vector>

namespace tvm {


inline std::string Type2String(const Type& t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

inline Type String2Type(std::string s) {
    std::istringstream is(s);
    halide_type_code_t code = Type::Int;
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

    int bits = 32, lanes = 0;
    if (sscanf(s.c_str(), "%dx%d", &bits, &lanes) == 0) {
        LOG(FATAL) << "unknow type " << s;
    }
    return Type(code, bits, lanes);
}



struct APIVariantValue {
    ArgVariantID type_id{kNull};
    std::shared_ptr<Node> sptr;
    std::string str;
    ArgVariant v_union;

    APIVariantValue() {}
    inline void Clear() {}

    inline APIVariantValue& operator=(double value) {
        type_id = kDouble;
        v_union.v_double = value;
        return *this;
    }

    inline APIVariantValue& operator=(std::nullptr_t value) {
        type_id = kNull;
        return *this;
    }

    inline APIVariantValue& operator=(int64_t value) {
        type_id = kLong;
        v_union.v_long = value;
        return *this;
    }

    inline APIVariantValue& operator=(std::string value) {
        type_id = kStr;
        str = std::move(value);
        v_union.v_str = str.c_str();
        return *this;
    }

    inline APIVariantValue& operator=(const NodeRef& ref) {
        type_id = kNodeHandle;
        this->sptr = ref.node_;
        return *this;
    }

    inline APIVariantValue& operator=(const Type& value) {
        return operator=(Type2String(value));
    }

    template<typename T,
            typename = typename std::enable_if<std::is_base_of<NodeRef, T>::value>::type>
    inline operator T() const {
        if (type_id == kNull) return T();
        CHECK_EQ(type_id, kNodeHandle);
        return T(sptr);
    }

    inline operator Expr() const {
        if (type_id == kNull) return Expr();
        if (type_id == kLong) return Expr(operator int());
        if (type_id == kDouble) {
            return Expr(static_cast<float>(operator double()));
        }
        CHECK_EQ(type_id, kNodeHandle);
        std::shared_ptr<Node> x = sptr;
        return Expr(sptr);
    }

    inline operator double() const {
        CHECK_EQ(type_id, kDouble);
        return v_union.v_double;
    }

    inline operator int64_t() const {
        CHECK_EQ(type_id, kLong);
        return v_union.v_long;
    }

    inline operator uint64_t() const {
        CHECK_EQ(type_id, kLong);
        return v_union.v_long;
    }

    inline operator int() const {
        CHECK_EQ(type_id, kLong);
        CHECK_LE(v_union.v_long, std::numeric_limits<int>::max());
        return v_union.v_long;
    }

    inline operator bool() const {
        CHECK_EQ(type_id, kLong);
        return v_union.v_long != 0;
    }

    inline operator std::string() const {
        CHECK_EQ(type_id, kStr);
        return str;
    }

    inline operator Type() const {
        return String2Type(operator std::string());
    }
};

using APIFunction = std::function<
    void(const std::vector<APIVariantValue> &args, APIVariantValue* ret)>;

struct APIFunctionReg : public dmlc::FunctionRegEntryBase<APIFunctionReg, APIFunction> {

};

#define TVM_REGISTER_API(TypeName)                                             \
    DMLC_REGISTRY_REGISTER(::tvm::APIFunctionReg, APIFunctionReg, TypeName)

}; // end of namespace tvm


#endif // TVM_C_API_C_API_REGISTRY_H_
