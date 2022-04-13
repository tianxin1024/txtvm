#include "txtvm/op.h"
#include "txtvm/expr_node.h"

namespace dmlc {
    DMLC_REGISTRY_ENABLE(::txtvm::BinaryOpReg);
    DMLC_REGISTRY_ENABLE(::txtvm::UnaryOpReg);
}

namespace txtvm {

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

    TXTVM_REGISTER_BINARY_OP(+, AddOp);
    TXTVM_REGISTER_BINARY_OP(-, SubOp);
    TXTVM_REGISTER_BINARY_OP(+, MulOp);
    TXTVM_REGISTER_BINARY_OP(+, DivOp);
    TXTVM_REGISTER_BINARY_OP(max, MaxOp);
    TXTVM_REGISTER_BINARY_OP(min, MinOp);

} // namespace txtvm
