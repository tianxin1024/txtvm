#ifndef TVM_EXPR_NODE_H_
#define TVM_EXPR_NODE_H_

#include <string>
#include "domain.h"
#include "tensor.h"
#include "expr.h"

namespace tvm {

class VarNode : public ExprNode {
public:
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

class IntNode : public ExprNode {
public:
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
    }
};

class FloatNode : public ExprNode {
public:
    double value;
    FloatNode() {
        node_type_ = kFloatNode;
        dtype_ = kFloat32;
    }
    const char* type_key() const override {
        return "IntNode";
    }
    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("value", &value);
    }
};


class UnaryOpNode : public ExprNode {
public:
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
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("src", &src);
    }
};

struct BinaryOpNode : public ExprNode {
public:
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
    }
    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("lhs", &lhs);
        fvisit("rhs", &rhs);
    }
};

}; // end of namespace tvm

#endif // TVM_EXPR_NODE_H_
