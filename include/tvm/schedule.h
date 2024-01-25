#ifndef TVM_SCHEDULE_H_
#define TVM_SCHEDULE_H_

#include <string>
#include "base.h"
#include "split.h"
#include "tensor.h"

namespace tvm {

class ScheduleNode;
class AttachSpecNode;

enum AttachType : int {
    kRoot = 0,
    kInline = 1,
    kSplit = 2
};

class Schedule : public NodeRef {
public:
    inline const ScheduleNode* operator->() const;
};

class AttachSpec : public NodeRef {
public:
    inline const AttachSpecNode* operator->() const;
};

class AttachSpecNode : public Node {
public:
    AttachType attach_type;
    Split attach_split;
    Schedule schedule;

    const char* type_key() const override {
        return "AttachSpecNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("attach_type", &attach_type);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("attach_split", &attach_split);
        fvisit("schedule", &schedule);
    }
};

class ScheduleNode : public Node {
public:
    Tensor tensor;
    std::string scope;
    Array<Split> splits;
    Array<AttachSpec> attachs;

    const char* type_key() const override {
        return "AttachSpecNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("scope", &scope);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("tensor", &tensor);
        fvisit("splits", &splits);
        fvisit("attachs", &attachs);
    }
};

inline const ScheduleNode* Schedule::operator->() const {
    return static_cast<const ScheduleNode*>(node_.get());
}

inline const AttachSpecNode* AttachSpec::operator->() const {
    return static_cast<const AttachSpecNode*>(node_.get());
}

}; // end of namespace tvm


#endif // TVM_SCHEDULE_H_
