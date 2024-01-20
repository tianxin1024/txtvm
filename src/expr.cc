#include <txtvm/expr.h>
#include <txtvm/op.h>
#include <txtvm/expr_node.h>

namespace tvm {

Var::Var(std::string name, DataType dtype) {
    auto node_ = std::make_shared<VarNode>();
    node_->name = std::move(name);
    node_->dtype_ = dtype;
}

Expr IntConstant(int64_t value) {
    auto nptr = std::make_shared<IntNode>();
    nptr->value = value;
    return Expr(std::move(nptr));
}

Expr FloatConstant(double value) {
    auto nptr = std::make_shared<FloatNode>();
    nptr->value = value;
    return Expr(std::move(nptr));
}

Expr operator+(Expr lhs, Expr rhs) {
    auto nptr = std::make_shared<BinaryOpNode>(AddOp::Get(), lhs, rhs);
    nptr->Verify();
    return Expr(std::move(nptr));
}

} // end of namespace tvm
