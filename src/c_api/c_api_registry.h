#ifndef TXTVM_C_API_C_API_REGISTRY_H_
#define TXTVM_C_API_C_API_REGISTRY_H_


#include "txtvm/base.h"
#include "txtvm/expr.h"
#include "txtvm/c_api.h"
#include <memory>
#include <string>
#include <vector>

namespace txtvm {

    /*! \brief Variant container for API calls */
    struct APIVariantValue {
        /*! \brief the type id */
        ArgVariantID type_id{kNull};
        /*! \brief shared pointer container */
        std::shared_ptr<Node> sptr;
        /*! \brief string container */
        std::string str;
        /*! \brief the variant holder */
        ArgVariant v_union;
        // constructor
        APIVariantValue() {}
        // clear value
        inline void Clear() {}

        // assign op
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
            v_union.v_str= str.c_str();
            return *this;
        }
        inline APIVariantValue& operator=(const NodeRef& ref) {
            type_id = kNodeHandle;
            this->sptr = ref.node_;
            return *this;
        }
        template<typename T,
                typename = typename std::enable_if<std::is_base_of<NodeRef, T>::value>::type >
        inline operator T() const {
            if (type_id == kNull) return T();
            CHECK_EQ(type_id, kNodeHandle);
            std::shared_ptr<Node> x = sptr;
            return T(std::move(x));
        }
        inline operator double() const {
            CHECK_EQ(type_id, kDouble);
            return v_union.v_double;
        }
        inline operator int64_t() const {
            CHECK_EQ(type_id, kLong);
            return v_union.v_long;
        }
        inline operator int() const {
            CHECK_EQ(type_id, kLong);
            return v_union.v_long;
        }
        inline operator std::string() const {
            CHECK_EQ(type_id, kStr);
            return str;
        }
    }; // struct end of APIVariantValue

    // common defintiion of API function.
    using APIFunction = std::function<
        void(const std::vector<APIVariantValue> &args, APIVariantValue* ret)>;

    /*!
     * \brief Registry entry for DataIterator factory functions.
     */
    struct APIFunctionReg
        : public dmlc::FunctionRegEntryBase<APIFunctionReg, APIFunction> {
    }; // struct end of APIFunctionReg

#define TXTVM_REGISTER_API(TypeName)                                          \
    DMLC_REGISTRY_REGISTER(::txtvm::APIFunctionReg, APIFunctionReg, TypeName) \


} // namespace txtvm

#endif // TXTVM_C_API_C_API_REGISTRY_H_
