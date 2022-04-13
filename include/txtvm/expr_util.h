#ifndef TXTVM_EXPR_UTIL_H_
#define TXTVM_EXPR_UTIL_H_

#include "./expr.h"
#include "./expr_node.h"

namespace txtvm {

    /*!
    * \brief simplify the expression src
    * \param src The source expression
    * \return the simplified expression.
    */
    Expr Simplify(const Expr& src);

    /*!
    * \brief visit the exression node in expr tree in post DFS order.
    * \param expr The expression tree
    * \param fvisit The visit function.
    */
    template<typename FVisit>
    inline void Visit(const Expr& expr, FVisit fvisit) {
        // TODO(tx) chaneg to stack based impl.
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
} // namespace txtvm


#endif // TXTVM_EXPR_UTIL_H_
