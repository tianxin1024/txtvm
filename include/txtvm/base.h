#ifndef TVM_BASE_H_
#define TVM_BASE_H_

#include <dmlc/logging.h>
#include <dmlc/registry.h>
#include <string>
#include <memory>
#include <functional>
#include <typeinfo>

namespace tvm {

// forward declarationi
class Node;
class NodeRef;
class UnaryOp;
class BinaryOp;

enum DataType {
    kUnknown,
    kInt32,
    kFloat32
};

enum NodeType {
    kVarNode,
    kIntNode,
    kFloatNode,
    kUnaryOpNode,
    kBinaryOpNode,
    kReduceNode,
    kTensorReadNode,
    kOtherNodes
};

class AttrVisitor {
public:
    virtual void Visit(const char* key, double* value) = 0;
    virtual void Visit(const char* key, int64_t* value) = 0;
    virtual void Visit(const char* key, DataType* value) = 0;
    virtual void Visit(const char* key, std::string* value) = 0;
    virtual void Visit(const char* key, const UnaryOp** value) = 0;
    virtual void Visit(const char* key, const BinaryOp** value) = 0;
};

using FNodeRefVisit = std::function<void(const char* key, NodeRef* ref)>;

class Node {
public:
    virtual ~Node();
    virtual const char* type_key() const = 0;
    virtual void Verify() const {}
    virtual void VisitAttrs(AttrVisitor* visitor) {}
    virtual void VisitNodeRefFields(FNodeRefVisit visitor) {}

    template<typename TNode>
    inline bool is_type() const;

    inline NodeType node_type() const;

protected:
    friend class NodeRef;
    NodeType node_type_{kOtherNodes};
};

class NodeRef {
public:
    template<typename TNode>
    inline const TNode* Get() const;
    inline bool is_null() const;

protected:
    NodeRef() = default;
    explicit NodeRef(std::shared_ptr<Node> node) : node_(node) {}
    std::shared_ptr<Node> node_;
};

using NodeFactory = std::function<std::shared_ptr<Node> ()>;

struct NodeFactoryReg : public dmlc::FunctionRegEntryBase<NodeFactoryReg, NodeFactory> {

};

#define TVM_REGISTER_NODE_TYEP(TypeName)                                    \
    DMLC_REGISTRY_REGISTER(::tvm::NodeFactoryReg, NodeFactoryReg, TypeName) \
    .set_body([]() { return std::make_shared<TypeName>(); })

inline NodeType Node::node_type() const {
    return node_type_;
}

template<typename TNode>
inline bool Node::is_type() const {
    const std::type_info& tinfo = typeid(*this);
    if (&typeid(TNode) == &tinfo) return true;
    return typeid(TNode) == tinfo;
}

template<typename TNode>
inline const TNode* NodeRef::Get() const {
    CHECK(node_->is_type<TNode>())
        << " type inconsistent, expected " << typeid(TNode).name()
        << " given " << typeid(*this).name();
    return static_cast<const TNode*>(node_.get());
}

inline bool NodeRef::is_null() const {
    return node_.get() == nullptr;
}

} // end of namespace tvm


#endif // TVM_BASE_H_
