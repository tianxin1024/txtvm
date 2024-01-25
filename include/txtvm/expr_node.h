#ifndef TVM_EXPR_NODE_H_
#define TVM_EXPR_NODE_H_

#include <string>
#include "domain.h"
#include "tensor.h"
#include "expr.h"

namespace tvm {

struct VarNode : public ExprNode {
    std::string name;
    VarNode() {
        node_type_ = kVarNode;
    }

    const char* type_key() const override {
        return "VarNode";
    }

    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("name", &name);
        visitor->Visit("dtype", &dtype_);
    }
};

struct IntNode : public ExprNode {
    int64_t value;
    IntNode() {
        node_type_ = kIntNode;
        dtype_ = kInt32;
    }
    const char* type_key() const override {
        return "IntNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("value", &value);
        visitor->Visit("dtype", &dtype_);
    }
};

struct FloatNode : public ExprNode {
    double value;
    FloatNode() {
        node_type_ = kFloatNode;
        dtype_ = kFloat32;
    }
    const char* type_key() const override {
        return "FloatNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("value", &value);
        visitor->Visit("dtype", &dtype_);
    }
};


struct UnaryOpNode : public ExprNode {
    const UnaryOp* op;
    Expr src;
    UnaryOpNode() {
        node_type_ = kUnaryOpNode;
    }
    UnaryOpNode(const UnaryOp* op, Expr && src) 
            : op(op), src(std::move(src)) {
        node_type_ = kUnaryOpNode;
        dtype_ = this->src.dtype();
    }

    ~UnaryOpNode() {
        this->Destory();
    }

    const char* type_key() const override {
        return "UnaryOpNode";
    }
    void Verify() const override {
        CHECK_EQ(dtype_, src.dtype());
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("op", &op);
        visitor->Visit("dtype", &dtype_);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("src", &src);
    }
};

struct BinaryOpNode : public ExprNode {
    const BinaryOp* op;
    Expr lhs;
    Expr rhs;
    BinaryOpNode() { 
        node_type_ = kBinaryOpNode;
    }
    BinaryOpNode(const BinaryOp* op, Expr && lhs, Expr && rhs) 
            : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {
        node_type_ = kBinaryOpNode;
        dtype_ = this->lhs.dtype();
    }

    ~BinaryOpNode() {
        this->Destory();
    }

    const char* type_key() const override {
        return "binaryOpNode";
    }
    void Verify() const override {
        CHECK_EQ(dtype_, rhs.dtype());
        CHECK_EQ(dtype_, lhs.dtype());
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("op", &op);
        visitor->Visit("dtype", &dtype_);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("lhs", &lhs);
        fvisit("rhs", &rhs);
    }
};

struct ReduceNode : public ExprNode {
    const BinaryOp* op;
    Expr src;
    RDomain rdom;
    ReduceNode() {
        node_type_ = kReduceNode;
    }
    ReduceNode(const BinaryOp* op, Expr && src, RDomain && rdom) 
            : op(op), src(std::move(src)), rdom(std::move(rdom)) {
        node_type_ = kReduceNode;
        dtype_ = this->src.dtype();
    }
    ~ReduceNode() {
        this->Destory();
    }
    const char* type_key() const override {
        return "ReduceNode";
    }
    void Verify() const override {
        CHECK_EQ(dtype_, src.dtype());
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("op", &op);
        visitor->Visit("dtype", &dtype_);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("src", &src);
        fvisit("rdom", &rdom);
    }
};

struct TensorReadNode : public ExprNode {
    Tensor tensor;
    Array<Expr> indices;
    TensorReadNode() {
        node_type_ = kTensorReadNode;
    }
    TensorReadNode(Tensor && tensor, Array<Expr> && indices)
            : tensor(std::move(tensor)), indices(std::move(indices)) {
        node_type_ = kReduceNode;
        dtype_ = tensor->dtype;
    }
    ~TensorReadNode() {
        this->Destory();
    }
    const char* type_key() const override {
        return "TensorReadNode";
    }
    void Verify() const override {
        CHECK_EQ(dtype_, tensor->dtype);
        for (size_t i = 0; i < indices.size(); ++i) {
            CHECK_EQ(indices[i].dtype(), kInt32);
        }
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("dtype", &dtype_);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("tensor", &tensor);
        fvisit("indices", &indices);
    }
};

struct BufferReadNode : public ExprNode {
    Var buffer;
    Expr offset;
    BufferReadNode() {
        node_type_ = kBufferReadNode;
    }
    const char* type_key() const override {
        return "BufferReadNode";
    }
    void Verify() const override {
        CHECK_EQ(dtype_, Ptr2DataType(buffer.dtype()));
        CHECK_EQ(offset.dtype(), kInt32);
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("dtype", &dtype_);
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("buffer", &buffer);
        fvisit("offset", &offset);
    }

};

}; // end of namespace tvm

#endif // TVM_EXPR_NODE_H_
