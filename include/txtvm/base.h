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

enum DataType : int {
    kUnknown = 0,
    kInt32 = 1,
    kFloat32 = 2
};

enum NodeType {
    // expr nodes
    kVarNode,
    kIntNode,
    kFloatNode,
    kUnaryOpNode,
    kBinaryOpNode,
    kReduceNode,
    kTensorReadNode,
    // stmt nodes
    kStoreNode,
    kForRangeNode,
    kIfThenElseNode,
    kOtherNodes
};

class AttrVisitor {
public:
    virtual void Visit(const char* key, double* value) = 0;
    virtual void Visit(const char* key, int64_t* value) = 0;
    virtual void Visit(const char* key, int* value) = 0;
    virtual void Visit(const char* key, std::string* value) = 0;
    virtual void Visit(const char* key, const UnaryOp** value) = 0;
    virtual void Visit(const char* key, const BinaryOp** value) = 0;

    template<typename ENum,
            typename = typename std::enable_if<std::is_enum<ENum>::value>::type>
    void Visit(const char* key, ENum* ptr) {
        static_assert(std::is_same<int, typename std::underlying_type<ENum>::type>::value,
                "declare enum to be enum int to use visitor");
        this->Visit(key, reinterpret_cast<int*>(ptr));
    }
};

using FNodeRefVisit = std::function<void(const char* key, NodeRef* ref)>;

class Node {
public:
    virtual ~Node() {}
    virtual const char* type_key() const = 0;
    virtual void Verify() const {}
    virtual void VisitAttrs(AttrVisitor* visitor) {}
    virtual void VisitNodeRefFields(FNodeRefVisit visitor) {}

    template<typename TNode>
    inline bool is_type() const;

protected:
    friend class NodeRef;
    void Destory();
    NodeType node_type_{kOtherNodes};
};

class NodeRef {
public:
    template<typename TNode>
    inline const TNode* Get() const;
    inline NodeType node_type() const;
    inline bool is_null() const;

    inline bool operator==(const NodeRef& other) const;
    inline bool operator!=(const NodeRef& other) const;
    inline size_t hash() const;

protected:
    template<typename T, typename>
    friend class Array;
    friend class APIVariantValue;
    friend class Node;
    NodeRef() = default;
    explicit NodeRef(std::shared_ptr<Node> node) : node_(std::move(node)) {}
    std::shared_ptr<Node> node_;
};

using NodeFactory = std::function<std::shared_ptr<Node> ()>;

struct NodeFactoryReg : public dmlc::FunctionRegEntryBase<NodeFactoryReg, NodeFactory> {

};

#define TVM_REGISTER_NODE_TYPE(TypeName)                                    \
    DMLC_REGISTRY_REGISTER(::tvm::NodeFactoryReg, NodeFactoryReg, TypeName) \
    .set_body([]() { return std::make_shared<TypeName>(); })

inline NodeType NodeRef::node_type() const {
    return node_->node_type_;
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

inline bool NodeRef::operator==(const NodeRef& other) const {
    return node_.get() == nullptr;
}

inline bool NodeRef::operator!=(const NodeRef& other) const {
    return node_.get() != other.node_.get();
}

inline size_t NodeRef::hash() const {
    return std::hash<Node*>()(node_.get());
}

} // end of namespace tvm

namespace std {
template <>
struct hash<::tvm::NodeRef> {
    std::size_t operator()(const ::tvm::NodeRef& k) const {
        return k.hash();
    }
};

} // end of namespace std


#endif // TVM_BASE_H_
