#ifndef TVM_DOMAIN_H_
#define TVM_DOMAIN_H_

#include <memory>
#include "base.h"
#include "array.h"
#include "expr.h"

namespace tvm {

class RangeNode;

class RDomainNode;

class Range : public NodeRef {
public:
    Range() {}
    Range(Expr begin, Expr end);
    Expr extent() const;

    inline const RangeNode* operator->() const;
    inline const Expr& begin() const;
    inline const Expr& end() const;

    // inline const Expr& begin() const {
    //     return static_cast<const RangeNode*>(node_.get())->begin;
    // }
    // inline const Expr& end() const {
    //     return static_cast<const RangeNode*>(node_.get())->end;
    // }
    
    friend std::ostream& operator<<(std::ostream &os, const Range& r) {
        os << '[' << r.begin() << ", " << r.end() << ']';
        return os;
    }
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

    inline const Domain& domain() const;

    friend std::ostream& operator<<(std::ostream &os, const RDomain& r) {
        os << "rdomain(" << r.domain() << ")";
        return os;
    }
};

using RDom = RDomain;

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

// implements of inline functions
inline const RangeNode* Range::operator->() const {
    return static_cast<const RangeNode*>(node_.get());
}

inline const Expr& Range::begin() const {
    return (*this)->begin;
}

inline const Expr& Range::end() const {
    return (*this)->end;
}

inline const RDomainNode* RDomain::operator->() const {
    return static_cast<const RDomainNode*>(node_.get());
}

inline size_t RDomain::ndim() const {
    return (*this)->index.size();
}

inline Var RDomain::index(size_t i) const {
    return (*this)->index[i];
}

inline const Domain& RDomain::domain() const {
    return (*this)->domain;
}

}; // end of namespace tvm

#endif // TVM_DOMAIN_H_
