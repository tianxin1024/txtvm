#ifndef TVM_OP_H_
#define TVM_OP_H_

#include <string>
#include "expr.h"

namespace tvm {

class BinaryOp {
public:
    virtual const char* FunctionName() const = 0;

    Expr operator()(Expr lhs, Expr rhs) const;
};

class UnaryOp {
public:
    virtual const char* FunctionName() const = 0;

    Expr operator()(Expr src) const;
};

class AddOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "+";
    }
    static AddOp* Get();
};

class SubOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "-";
    }
    static SubOp* Get();
};

class MulOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "*";
    }
    static MulOp* Get();
};

class DivOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "/";
    }
    static DivOp* Get();
};

class MaxOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "max";
    }
    static MaxOp* Get();
};


class MinOp : public BinaryOp {
public:
    const char* FunctionName() const override {
        return "min";
    }
    static MinOp* Get();
};

#define DEFINE_OP_OVERLOAD(OpChar, OpName)                \
    inline Expr operator OpChar (Expr lhs, Expr rhs) {    \
        return (*OpName::Get())(lhs, rhs);                \
    }

#define DEFINE_BINARY_OP_FUNCTION(FuncName, OpName)       \
    inline Expr FuncName(Expr lhs, Expr rhs) {            \
        return (*OpName::Get())(lhs, rhs);                \
    }

DEFINE_OP_OVERLOAD(+, AddOp);
DEFINE_OP_OVERLOAD(-, AddOp);
DEFINE_OP_OVERLOAD(*, AddOp);
DEFINE_OP_OVERLOAD(/, AddOp);

DEFINE_BINARY_OP_FUNCTION(max, MaxOp);
DEFINE_BINARY_OP_FUNCTION(min, MaxOp);

inline Expr operator-(Expr src) {
    return src * (-1);
}

}; // end of namespace tvm


#endif // TVM_OP_H_
