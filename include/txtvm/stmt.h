#ifndef TVM_STMT_H_
#define TVM_STMT_H_

#include <type_traits>
#include "base.h"
#include "domain.h"

namespace tvm {

class Stmt : public NodeRef {
public:
    Stmt() {}

    explicit Stmt(std::shared_ptr<Node>&& nptr) : NodeRef(std::move(nptr)) {
        CHECK(node_.get() != nullptr);
    }
};

Stmt Store(Var buffer, Expr offset, Expr src);

Stmt ForRange(Var loop_var, Range range, Stmt body);

Stmt IfThenElse(Expr cond, Stmt then_body, Stmt else_body);

}; // end of namespace tvm

#endif // TVM_STMT_H_
