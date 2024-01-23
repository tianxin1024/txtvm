#ifndef TVM_OP_H_
#define TVM_OP_H_

#include <string>
#include "expr.h"

namespace tvm {

class BinaryOp {
public:
    virtual ~BinaryOp() {}
    virtual const char* FunctionName() const = 0;

    Expr operator()(Expr lhs, Expr rhs) const;

    static const BinaryOp* Get(const char* name);
};

class UnaryOp {
public:
    virtual const char* FunctionName() const = 0;

    Expr operator()(Expr src) const;

    static const UnaryOp* Get(const char* name);
};

class AddOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "+";
    }
};

class SubOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "-";
    }
};

class MulOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "*";
    }
};

class DivOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "/";
    }
};

class MaxOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "max";
    }
};


class MinOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "min";
    }
};

#define DEFINE_BINARY_OP_OVERLOAD(OpChar)                     \
    inline Expr operator OpChar (Expr lhs, Expr rhs) {        \
        static const BinaryOp* op = BinaryOp::Get(#OpChar);   \
        return (*op)(lhs, rhs);                               \
    }

#define DEFINE_BINARY_OP_FUNCTION(FuncName)                   \
    inline Expr FuncName(Expr lhs, Expr rhs) {                \
        static const BinaryOp* op = BinaryOp::Get(#FuncName); \
        return (*op)(lhs, rhs);                               \
    }

DEFINE_BINARY_OP_OVERLOAD(+);
DEFINE_BINARY_OP_OVERLOAD(-);
DEFINE_BINARY_OP_OVERLOAD(*);
DEFINE_BINARY_OP_OVERLOAD(/);

DEFINE_BINARY_OP_FUNCTION(max);
DEFINE_BINARY_OP_FUNCTION(min);

inline Expr operator-(Expr src) {
    return src * (-1);
}

template<typename Op>
struct OpReg {
    std::string name;
    std::unique_ptr<Op> op;
    inline OpReg& set(Op* op) {
        this->op.reset(op);
        return *this;
    }
};

using UnaryOpReg  = OpReg<UnaryOp>;
using BinaryOpReg = OpReg<BinaryOp>;

#define TVM_REGISTER_BINARY_OP(FunctionName, TypeName)                                 \
    static DMLC_ATTRIBUTE_UNUSED ::tvm::BinaryOpReg & __make_ ## _BinOp_ ## TypeName = \
    ::dmlc::Registry<::tvm::BinaryOpReg>::Get()->__REGISTER_OR_GET__(#FunctionName)    \
        .set(new TypeName())

#define TVM_REGISTER_UNARY_OP(FunctionName, TypeName)                                  \
    static DMLC_ATTRIBUTE_UNUSED ::tvm::BinaryOpReg & __make_ ## _BinOp_ ## TypeName = \
    ::dmlc::Registry<::tvm::UnaryOpReg>::Get()->__REGISTER_OR_GET__(#FunctionName)     \
        .set(new TypeName())


}; // end of namespace tvm


#endif // TVM_OP_H_
