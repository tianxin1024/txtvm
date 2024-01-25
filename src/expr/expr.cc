#include <txtvm/expr.h>
#include <txtvm/op.h>
#include <txtvm/expr_node.h>

namespace tvm {


Var::Var(std::string name, DataType dtype) {
    auto node = std::make_shared<VarNode>();
    node->name = std::move(name);
    node->dtype_ = dtype;
    node_ = std::move(node);
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

Expr BufferRead(Var buffer, Expr offset) {
    auto nptr = std::make_shared<BufferReadNode>();
    nptr->buffer = std::move(buffer);
    nptr->offset = std::move(offset);
    nptr->Verify();
    return Expr(std::move(nptr));
}

} // end of namespace tvm
