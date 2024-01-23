#include <txtvm/op.h>
#include <txtvm/expr_node.h>

namespace dmlc {
DMLC_REGISTRY_ENABLE(::tvm::BinaryOpReg);
DMLC_REGISTRY_ENABLE(::tvm::UnaryOpReg);
}; // end of namespace dmlc

namespace tvm {

Expr BinaryOp::operator()(Expr lhs, Expr rhs) const {
    auto nptr = std::make_shared<BinaryOpNode>(
            this, std::move(lhs), std::move(rhs));
    nptr->Verify();
    return Expr(std::move(nptr));
}

Expr BinaryOp::Reduce(Expr src, RDomain rdom) const {
    auto nptr = std::make_shared<ReduceNode>(
        this, std::move(src), std::move(rdom));
    nptr->Verify();
    return Expr(std::move(nptr));
}

const BinaryOp* BinaryOp::Get(const char* name) {
    const auto* op = dmlc::Registry<BinaryOpReg>::Find(name);
    CHECK(op != nullptr) << "cannot find " << name;
    return op->op.get();
}

TVM_REGISTER_BINARY_OP(+, AddOp);
TVM_REGISTER_BINARY_OP(-, SubOp);
TVM_REGISTER_BINARY_OP(*, MulOp);
TVM_REGISTER_BINARY_OP(/, DivOp);
TVM_REGISTER_BINARY_OP(max, MaxOp);
TVM_REGISTER_BINARY_OP(min, MinOp);

} // end of namespace tvm
