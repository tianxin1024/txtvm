#ifndef TVM_EXPR_UTIL_H_
#define TVM_EXPR_UTIL_H_

#include "expr.h"
#include "expr_node.h"

namespace tvm {

inline Expr Simplify(Expr src) {
    return src;
}

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
        default: break;
    }
    fvisit(expr);
}


}; // end of namespace tvm

#endif // !TVM_EXPR_UTIL_H_
