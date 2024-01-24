#ifndef TVM_SPLIT_H_
#define TVM_SPLIT_H_

#include "base.h"
#include "array.h"
#include "domain.h"

namespace tvm {

class SplitNode;

class Split : public NodeRef {
public:
    Split() {}
    inline bool is_over_rdom() const;
    inline const SplitNode* operator->() const;
};

class SplitNode : public Node {
public:
    int split_over_rdom{0};
    virtual Domain InferInnerDomain(Expr split_index, Domain out_domain) const = 0;
};

class DimSplitNode : public SplitNode {
public:
    int64_t dim_index;
    Expr factor;
    DimSplitNode() {}
    const char* type_key() const override {
        return "DimSplitNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("split_over_rdom", &split_over_rdom);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("factor", &factor);
    }
    Domain InferInnerDomain(Expr split_index, Domain out_domain) const override {
        LOG(FATAL) << "not implemented";
        return Domain();
    }
};

inline const SplitNode* Split::operator->() const {
    return static_cast<const SplitNode*>(node_.get());
}

inline bool Split::is_over_rdom() const {
    return (*this)->split_over_rdom != 0;
}

}; // end of namespace tvm

#endif // TVM_SPLIT_H_
