#ifndef TXTVM_OP_H
#define TXTVM_OP_H

#include <string>
#include "./expr.h"

namespace txtvm {

    class BinaryOp {
    public:
        /*! \return the function name to be called in binary op */
        virtual const char* FunctionName() const = 0;

       /*!
        * \brief apply the binary op
        * \param lhs left operand
        * \param rhs right operand
        * \return the result expr
        */
        Expr operator()(Expr lhs, Expr rhs) const;
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
    }; // class end of UnaryOp

    class AddOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "+";
        }
        static AddOp* Get();

    };  // class end of AddOp

    class SubOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "-";
        }
        static SubOp* Get();

    };  // class end of SubOp

    class MulOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "*";
        }
        static MulOp* Get();

    };  // class end of MulOp


    class DivOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "/";
        }
        static DivOp* Get();

    };  // class end of DivOp

    class MaxOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "max";
        }
        static MaxOp* Get();

    };  // class end of MaxOp

    class MinOp : public BinaryOp {
    public:
        const char* FunctionName() const override {
            return "min";
        }
        static MinOp* Get();

    };  // class end of MinOp


#define DEFINE_OP_OVERLOAD(OpChar, OpName)               \
    inline Expr operator OpChar(Expr lhs, Expr rhs) {    \
        return (*OpName::Get())(lhs, rhs);               \
    }

#define DEFINE_BINARY_OP_FUNCTION(FuncName, OpName)      \
    inline Expr FuncName(Expr lhs, Expr rhs) {           \
        return (*OpName::Get())(lhs, rhs);               \
    }

    DEFINE_OP_OVERLOAD(+, AddOp);
    DEFINE_OP_OVERLOAD(-, SubOp);
    DEFINE_OP_OVERLOAD(*, MulOp);
    DEFINE_OP_OVERLOAD(/, DivOp);

    DEFINE_BINARY_OP_FUNCTION(max, MaxOp);
    DEFINE_BINARY_OP_FUNCTION(min, MinOp);


    // overload negation
    inline Expr operator-(Expr src) {
        return src * (-1);
    }

} // namespace txtvm


#endif  // TXTVM_OP_H
