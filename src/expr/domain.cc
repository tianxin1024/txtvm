#include "txtvm/domain.h"
#include "txtvm/op.h"
#include "txtvm/expr_node.h"
#include "txtvm/expr_util.h"

namespace tvm {

Range::Range(Expr begin, Expr end) {
    node_ = std::make_shared<RangeNode>(
        std::move(begin), std::move(end));
}

Expr Range::extent() const {
    return Simplify(end() - begin());
}

RDomain::RDomain(Domain domain) {
    std::vector<Var> index;
    for (size_t i = 0; i < domain.size(); ++i) {
        index.push_back(Var("reduction_index"));
    }
    Array<Var> idx(index);
    node_ = std::make_shared<RDomainNode>(
        std::move(idx), std::move(domain));
}

TVM_REGISTER_NODE_TYPE(RangeNode);
TVM_REGISTER_NODE_TYPE(ArrayNode);
TVM_REGISTER_NODE_TYPE(RDomainNode);

}; // end of namespce tvm
