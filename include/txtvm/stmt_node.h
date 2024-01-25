#ifndef TVM_STMT_NODE_H_
#define TVM_STMT_NODE_H_

#include "base.h"
#include "expr.h"
#include "domain.h"

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

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("buffer", &buffer);
        fvisit("offset", &offset);
        fvisit("src", &src);
    }
};

struct ForRangeNode : public StmtNode {
    Var loop_var;
    Range range;
    Stmt body;

    ForRangeNode() {
        node_type_ = kForRangeNode;
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("loop_var", &loop_var);
        fvisit("range", &range);
        fvisit("body", &body);
    }
};

struct IfThenElseNode : public StmtNode {
    Expr cond;
    Expr then_body;
    Stmt else_body;

    IfThenElseNode() {
        node_type_ = kIfThenElseNode;
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("cond", &cond);
        fvisit("then_body", &then_body);
        fvisit("else_body", &else_body);
    }
};



}; // end of namespace tvm

#endif // TVM_STMT_NODE_H_
