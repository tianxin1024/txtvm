#ifndef TVM_CODEGEN_H_
#define TVM_CODEGEN_H_

#include "base.h"
#include "expr.h"

namespace tvm {

struct Assign : public Node {
    Expr src;
    Expr offset;
    Var ptr;
};


struct Loop : public Node {
    Expr init;
    Expr cond;
    Var body;
};


struct IfThenElse : public Node {
    Expr cond;
    Expr then_;
    Var else_;
};

}; // end of namespace tvm

#endif // TVM_CODEGEN_H_
