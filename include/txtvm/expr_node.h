#ifndef TXTVM_EXPR_NODE_H_
#define TXTVM_EXPR_NODE_H_

#include <string>
#include "./domain.h"
#include "./tensor.h"
#include "./expr.h"

namespace txtvm {

    /*! \brief variable node for symbolic variables */
    class VarNode : public ExprNode {
    public:
        /*! \brief hint name of the variable */
        std::string name;
        /*! \brief constructor */
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
    }; // class end of VarNode


    /*! \brief integer constant node */
    class IntNode : public ExprNode {
    public:
        /*! \brief the value field */
        int64_t value;
        /*! \brief constructor */
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
    }; // class end of IntNode

    /*! \brief float constant node */
    class FloatNode : public ExprNode {
    public:
        /*! \brief the value field */
        double value;
        /*! \brief constructor */
        FloatNode() {
            node_type_ = kFloatNode;
            dtype_ = kFloat32;
        }
        const char* type_key() const override {
            return "FloatNode";
        }
        void VisitAttrs(AttrVisitor* visitor) override {
            visitor->Visit("value", &value);
        }
    }; // class end of FloatNode

    /*! \brief Unary mapping operator */
    class UnaryOpNode : public ExprNode {
    public:
        /*! \brief The operator */
        const UnaryOp* op;
        /*! \brief The source expression */
        Expr src;
        /*! \brief constructor */
        UnaryOpNode() {
            node_type_ = kUnaryOpNode;
        }
        UnaryOpNode(const UnaryOp* op, Expr && src) 
            : op(op), src(std::move(src)) {
            node_type_ = kUnaryOpNode;
            dtype_ = this->src.dtype();
        }
        ~UnaryOpNode() {
            this->Destroy();
        }
        const char* type_key() const override {
            return "kUnaryOpNode";
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
    }; // class end of FloatNode

    /*! \brief Binary mapping operator */
    class BinaryOpNode : public ExprNode {
    public:
        /*! \brief The operator */
        const BinaryOp* op;
        /*! \brief THe left expression */
        Expr lhs;
        /*! \brief THe right expression */
        Expr rhs;
        /*! \brief constructor, no not use constructor */
        BinaryOpNode() {
            node_type_ = kBinaryOpNode;
        }
        BinaryOpNode(const BinaryOp* op, Expr && lhs, Expr && rhs) 
                : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {
            node_type_ = kBinaryOpNode;
            dtype_ = this->lhs.dtype();
        }
        ~BinaryOpNode() {
            this->Destroy();
        }
        const char* type_key() const override {
            return "BinaryOpNode";
        }
        void Verify() const override {
            CHECK_EQ(dtype_, lhs.dtype());
            CHECK_EQ(dtype_, rhs.dtype());
        }
        void VisitAttrs(AttrVisitor* visitor) override {
            visitor->Visit("op", &op);
        }
        void VisitNodeRefFields(FNodeRefVisit fvisit) override {
            fvisit("lhs", &lhs);
            fvisit("rhs", &rhs);
        }
    }; // class end of FloatNode


} // namespace txtvm


#endif // TXTVM_EXPR_NODE_H_
