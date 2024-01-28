#ifndef TVM_DOMAIN_H_
#define TVM_DOMAIN_H_

#include "ir/Range.h"
#include <memory>
#include "base.h"
#include "expr.h"

namespace tvm {

class RDomainNode;

class Range : public Halide::IR::Range {
public:
    Range() {}

    explicit Range(std::shared_ptr<Node> n) : Halide::IR::Range(n) {}

    Range(Expr begin, Expr end);
};

using Domain = Array<Range>;

class RDomain : public NodeRef {
public:
    RDomain() {}

    explicit RDomain(Domain domain);

    explicit RDomain(std::initializer_list<Range> domain)
        : RDomain(Domain(domain)) {}

    inline const RDomainNode* operator->() const;

    inline size_t ndim() const;

    inline Var index(size_t i) const;

    inline Var i0() const {
        return index(0);
    }
}; 

using RDom = RDomain;

class RDomainNode : public Node {
public:
    Array<Var> index;
    Domain domain;
    RDomain domain;
    RDomainNode() {}
    RDomainNode(Array<Var> && index, Domain && domain) 
        : index(std::move(index)), domain(std::move(domain)) {
    }
    const char* type_key() const override {
        return "RDomain";
    }
    void VisitAttrs(AttrVisitor* v) final {
        v->Visit("index", &index);
        v->Visit("domain", &domain);
    }
};

inline const RDomainNode* RDomain::operator->() const {
    return static_cast<const RDomainNode*>(node_.get());
}

inline size_t RDomain::ndim() const {
    return (*this)->index.size();
}

// overload print function
inline std::ostream& operator<<(std::ostream &os, const RDomain& r) {
    os << "rdomain(" << r->domain << ")";
    return os;
}

}; // end of namespace tvm

#endif // TVM_DOMAIN_H_
