#ifndef TVM_ARRAY_H_
#define TVM_ARRAY_H_

#include <type_traits>
#include <vector>
#include <initializer_list>

#include "base.h"

namespace tvm {

class ArrayNode : public Node {
public:
    std::vector<std::shared_ptr<Node>> data;
    const char* type_key() const override {
        return "ArrayNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        LOG(FATAL) << "need to specially handle list";
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        LOG(FATAL) << "need to specially handle list";
    }
};

template<typename T,
         typename = typename std::enable_if<std::is_base_of<NodeRef, T>::value>::type >
class Array : public NodeRef {
public:
    Array() {}

    Array(Array<T> && other) {
        node_ = std::move(other.node_);
    }

    Array(const Array<T> &other) {
        node_ = other.node_;
    }

    template<typename IterType>
    Array(IterType begin, IterType end) {
        assign(begin, end);
    }

    Array(std::initializer_list<T> init) {
        assign(init.begin(), init.end());
    }

    Array(const std::vector<T>& init) {
        assign(init.begin(), init.end());
    }

    Array<T>& operator=(Array<T> && other) {
        node_ = std::move(other.node_);
        return *this;
    }

    Array<T>& operator=(const Array<T> & other) {
        node_ = std::move(other.node_);
        return *this;
    }

    template<typename IterType>
    void assign(IterType begin, IterType end) {
        auto n = std::make_shared<ArrayNode>();
        n->data.reserve(end - begin);
        for (IterType i = begin; i < end; ++i) {
            n->data.push_back(i->node_);
        }
        node_ = std::move(n);
    }

    inline T operator[](size_t i) const {
        T inst;
        inst.node_ = static_cast<const ArrayNode*>(node_.get())->data[i];
        return inst;
    }

    inline size_t size() const {
        if (node_.get() == nullptr) return 0;
        return static_cast<const ArrayNode*>(node_.get())->data.size();
    }

};
    

}; // end of namespace tvm

#endif // !TVM_ARRAY_H_
