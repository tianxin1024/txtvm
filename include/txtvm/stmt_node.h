#ifndef TVM_STMT_NODE_H_
#define TVM_STMT_NODE_H_

#include "base.h"
#include "expr.h"
#include "domain.h"
#include "stmt.h"

namespace tvm {

struct StmtNode : public Node {
};

struct StoreNode : public StmtNode {
    Var buffer;
    Expr offset;
    Expr src;
    StoreNode() {
        node_type_ = kStoreNode;
    }

    const char* type_key() const override {
        return "StoreNode";
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("buffer", &buffer);
        fvisit("offset", &offset);
        fvisit("src", &src);
    }

    void Verify() const override {
        CHECK_EQ(Ptr2DataType(buffer.dtype()), src.dtype());
        CHECK_EQ(offset.dtype(), kInt32);
    }
};

struct ForRangeNode : public StmtNode {
    Var loop_var;
    Range range;
    Stmt body;

    ForRangeNode() {
        node_type_ = kForRangeNode;
    }

    const char* type_key() const override {
        return "ForRangeNode";
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("loop_var", &loop_var);
        fvisit("range", &range);
        fvisit("body", &body);
    }

    void Verify() const override {
        CHECK_EQ(loop_var.dtype(), kInt32);
        CHECK_EQ(this->range->begin.dtype(), loop_var.dtype());
        CHECK_EQ(this->range->end.dtype(), loop_var.dtype());
    }
};

struct IfThenElseNode : public StmtNode {
    Expr cond;
    Expr then_body;
    Stmt else_body;

    IfThenElseNode() {
        node_type_ = kIfThenElseNode;
    }

    const char* type_key() const override {
        return "IfThenElseNode";
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("cond", &cond);
        fvisit("then_body", &then_body);
        fvisit("else_body", &else_body);
    }

    void Verify() const override {
        CHECK_EQ(cond.dtype(), kInt32);
    }
};


}; // end of namespace tvm

#endif // TVM_STMT_NODE_H_
