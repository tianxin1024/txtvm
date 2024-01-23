#include <txtvm/expr.h>
#include <txtvm/op.h>
#include <txtvm/expr_node.h>

namespace tvm {

void Expr::Print(std::ostream& os) const {
    if (is_null()) {
        os << "null"; return;
    }
    switch (this->node_type()) {
        case kVarNode: {
            os << Get<VarNode>()->name; return;
        }
        case kIntNode: {
            os << Get<IntNode>()->value; return;
        }
        case kFloatNode: {
            os << Get<FloatNode>()->value; return;
        }
        case kBinaryOpNode: {
            const auto* n = Get<BinaryOpNode>();
            const char* fname = n->op->FunctionName();
            if (fname[1] == '\0' && !isalpha(fname[0])) {
                os << '(';
                n->lhs.Print(os);
                os << ' ' << fname[0] << ' ';
                n->rhs.Print(os);
                os << ')';
            } else {
                os << fname << '(';
                n->lhs.Print(os);
                os << ", ";
                n->rhs.Print(os);
                os << ')';
            }
            return ;
        }
        case kUnaryOpNode: {
            const auto* n = Get<UnaryOpNode>();
            os << n->op->FunctionName() << '(';
            n->src.Print(os);
            os << ')';
            return ;
        }
        case kReduceNode: {
            const auto* n = Get<ReduceNode>();
            os << "reduce(" << n->op->FunctionName() << ", ";
            n->src.Print(os);
            os << ", " << n->rdom << ')';
            return ;
        }
        case kTensorReadNode: {
            const auto* n = Get<TensorReadNode>();
            os << n->tensor.name() << n->indices;
            return ;
        }
        default: {
            LOG(FATAL) << "not able to handle type " << typeid(node_.get()).name();
        }
    }
}

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

} // end of namespace tvm
