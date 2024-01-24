#ifndef TVM_EXPR_UTIL_H_
#define TVM_EXPR_UTIL_H_

#include "expr.h"
#include "expr_node.h"

namespace tvm {

Expr Simplify(Expr src);

template<typename FVisit>
inline void Visit(const Expr& expr, FVisit fvisit) {
    switch (expr.node_type()) {
        case kBinaryOpNode: {
            const auto* n = expr.Get<BinaryOpNode>();
            Visit(n->lhs, fvisit);
            Visit(n->rhs, fvisit);
            break;
        }
        case kUnaryOpNode: {
            const auto* n = expr.Get<UnaryOpNode>();
            Visit(n->src, fvisit);
            break;
        }
        case kReduceNode: {
            const auto* n = expr.Get<ReduceNode>();
            Visit(n->src, fvisit);
            break;
        }
        case kTensorReadNode: {
            const auto* n = expr.Get<TensorReadNode>();
            for (size_t i = 0; i < n->indices.size(); ++i) {
                Visit(n->indices[i], fvisit);
            }
            break;
        }
        default: break;
    }
    fvisit(expr);
}


}; // end of namespace tvm

#endif // !TVM_EXPR_UTIL_H_
