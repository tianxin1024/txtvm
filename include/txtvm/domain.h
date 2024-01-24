#ifndef TVM_DOMAIN_H_
#define TVM_DOMAIN_H_

#include <memory>
#include "base.h"
#include "array.h"
#include "expr.h"

namespace tvm {

class RangeNode : public Node {
public:
    Expr begin;
    Expr end;
    RangeNode() {}
    RangeNode(Expr && begin, Expr && end) 
            : begin(std::move(begin)), end(std::move(end)) {
    }
    const char* type_key() const override {
        return "RangeNode";
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("begin", &begin);
        fvisit("end", &end);
    }
    void VisitAttrs(AttrVisitor* visitor) override {}
};

class Range : public NodeRef {
public:
    Range() {}
    Range(Expr begin, Expr end);
    Expr extent() const;
    inline const Expr& begin() const {
        return static_cast<const RangeNode*>(node_.get())->begin;
    }
    inline const Expr& end() const {
        return static_cast<const RangeNode*>(node_.get())->end;
    }
    friend std::ostream& operator<<(std::ostream &os, const Range& r) {
        os << '[' << r.begin() << ", " << r.end() << ']';
        return os;
    }
};

using Domain = Array<Range>;

class RDomainNode : public Node {
public:
    Array<Var> index;
    Domain domain;
    RDomainNode() {}

    RDomainNode(Array<Var> && index, Domain && domain) 
            : index(std::move(index)), domain(std::move(domain)) {
    }

    const char* type_key() const override {
        return "RDomainNode";
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("index", &index);
        fvisit("domain", &domain);
    }

    void VisitAttrs(AttrVisitor* visitor) override {}
};

class RDomain : public NodeRef {
public:
    RDomain() {}
    explicit RDomain(Domain domain);

    explicit RDomain(std::initializer_list<Range> domain)
            : RDomain(Domain(domain)) {}

    explicit RDomain(std::shared_ptr<Node>&& nptr) : NodeRef(std::move(nptr)) {
        CHECK(node_.get() != nullptr);
        CHECK(node_->is_type<RDomainNode>());
    }

    inline size_t ndim() const {
        return static_cast<const RDomainNode*>(node_.get())->index.size();
    }

    inline Var i0() const {
        return index(0);
    }

    inline Var index(size_t i) const {
        return static_cast<const RDomainNode*>(node_.get())->index[i];
    }

    inline const Domain& domain() const {
        return static_cast<const RDomainNode*>(node_.get())->domain;
    }

    friend std::ostream& operator<<(std::ostream &os, const RDomain& r) {
        os << "rdomain(" << r.domain() << ")";
        return os;
    }
};

}; // end of namespace tvm

#endif // TVM_DOMAIN_H_
