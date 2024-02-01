#include <dmlc/logging.h>
#include <gtest/gtest.h>
#include <tvm/tvm.h>
#include <tvm/ir_functor.h>
#include <tvm/ir_functor_ext.h>

TEST(IRF, Basic) {
    using namespace tvm;
    using namespace tvm::ir;
    Var x("x");
    auto z = x + 1;

    IRFunctor<int(const NodeRef& n, int b)> f;
    LOG(INFO) << "x";
    f.set_dispatch<Variable>([](const Variable* n, int b) {
        return b;
    });
    f.set_dispatch<Add>([](const Add* n, int b) {
        return b + 2;
    });
    CHECK_EQ(f(x, 2), 2);
    CHECK_EQ(f(z, 2), 4);
}

TEST(IRF, ExprTransform) {
    using namespace tvm;
    using namespace tvm::ir;
    Var x("x");
    auto z = x + 1;

    class MyExprFunctor
        : public ir::ExprFunctor<int(const Expr&, int)> {
    public:
        int VisitExpr_(const Variable* op, int b) final {
            return b;
        }
        int VisiExpr_(const IntImm* op , int b) {
            return op->value;
        }
        int VisiExpr_(const Add* op, int b) {
            return VisitExpr(op->a, b) + VisitExpr(op->b, b);
        }
    };

    MyExprFunctor f;
    CHECK_EQ(f(x, 2), 2);
    LOG(INFO) << "z: " << z;
    LOG(INFO) << "f(z, 2): " << f(z, 2); 
    CHECK_EQ(f(z, 2), 3);

    // try {
    //     f(z - 1, 2);
    //     LOG(FATAL) << "should fail";
    // } catch(dmlc::Error) {
    // }
}

TEST(IRF, ExprVisit) {
    using namespace tvm;
    using namespace tvm::ir;
    Var x("x");
    auto z = x + 1;

    class MyVisitor
        : public ir::ExprFunctor<void(const Expr&)>,
          public ir::StmtFunctor<void(const Stmt&)> {
    public:
        int count = 0;

        void VisitExpr_(const Variable* op) final {
            ++count;
        }
        void VisiExpr_(const IntImm* op) {
        }
        void VisiExpr_(const Add* op) {
            VisitExpr(op->a);
            VisitExpr(op->b);
        }
        void VisitStmt_(const Evaluate* op) final {
            VisitExpr(op->value);
        }
    };
    MyVisitor v;
    v.VisitStmt(Evaluate::make(z));
    CHECK_EQ(v.count, 1);
}


int main (int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "threadsafe";
    return RUN_ALL_TESTS();
}

