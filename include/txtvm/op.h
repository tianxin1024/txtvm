#ifndef TXTVM_OP_H_
#define TXTVM_OP_H_

#include <dmlc/registry.h>
#include <string>
#include "./expr.h"

namespace txtvm {

    class BinaryOp {
    public:
        // virtual destructor
        virtual ~BinaryOp() {}
        /*! \return the function name to be called in binary op */
        virtual const char* FunctionName() const = 0;
        /*!
        * \brief apply the binary op
        * \param lhs left operand
        * \param rhs right operand
        * \return the result expr
        */
        Expr operator()(Expr lhs, Expr rhs) const;
        /*!
        * \brief get binary op by name
        * \param name name of operator
        */
        static const BinaryOp* Get(const char* name);
    }; // class end of BinaryOp

    class UnaryOp {
    public:
        /*! \return the function name to be called in unary op */
        virtual const char* FunctionName() const = 0;
        /*!
        * \brief apply the unary op
        * \param src left operand
        * \return the result expr
        */
        Expr operator()(Expr src) const;
        /*!
        * \brief get unary op by name
        * \param name name of operator
        */
        static const UnaryOp* Get(const char* name);
    }; // class end of UnaryOp

    class AddOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "+";
        }

    };  // class end of AddOp

    class SubOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "-";
        }

    };  // class end of SubOp

    class MulOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "*";
        }

    };  // class end of MulOp


    class DivOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "/";
        }

    };  // class end of DivOp

    class MaxOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "max";
        }

    };  // class end of MaxOp

    class MinOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "min";
        }

    };  // class end of MinOp


#define DEFINE_BINARY_OP_OVERLOAD(OpChar)                   \
    inline Expr operator OpChar(Expr lhs, Expr rhs) {        \
        static const BinaryOp* op = BinaryOp::Get(#OpChar);  \
        return (*op)(lhs, rhs);                   \
    }

#define DEFINE_BINARY_OP_FUNCTION(FuncName)            \
    inline Expr FuncName(Expr lhs, Expr rhs) {                 \
        static const BinaryOp* op = BinaryOp::Get(#FuncName);  \
        return (*op)(lhs, rhs);                     \
    }

    DEFINE_BINARY_OP_OVERLOAD(+);
    DEFINE_BINARY_OP_OVERLOAD(-);
    DEFINE_BINARY_OP_OVERLOAD(*);
    DEFINE_BINARY_OP_OVERLOAD(/);

    DEFINE_BINARY_OP_FUNCTION(max);
    DEFINE_BINARY_OP_FUNCTION(min);


    // overload negation
    inline Expr operator-(Expr src) {
        return src * (-1);
    }

    // template of op registry
    template<typename Op>
    struct OpReg {
        std::string name;
        std::unique_ptr<Op> op;
        inline OpReg& set(Op* op) {
            this->op.reset(op);
            return *this;
        }
    };  // struct end of OpReg

    using UnaryOpReg = OpReg<UnaryOp>;
    using BinaryOpReg = OpReg<BinaryOp>;


#define TXTVM_REGISTER_BINARY_OP(FunctionName, TypeName)              \
    static DMLC_ATTRIBUTE_UNUSED ::txtvm::BinaryOpReg & __make_ ## _BinOp_ ## TypeName = \
    ::dmlc::Registry<::txtvm::BinaryOpReg>::Get()->__REGISTER_OR_GET__(#FunctionName)    \
        .set(new TypeName())


#define TXTVM_REGISTER_UNARY_OP(FunctionName, TypeName)               \
    static DMLC_ATTRIBUTE_UNUSED ::txtvm::BinaryOpReg & __make_ ## _BinOp_ ## Typename = \
    ::dmlc::Registry<::txtvm::UnaryOpReg>::Get()->__REGISTER_OR_GET__(#FunctionName)     \
        .set(new TypeName())

} // namespace txtvm


#endif  // TXTVM_OP_H_
