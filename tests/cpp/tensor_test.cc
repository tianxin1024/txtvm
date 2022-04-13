#include <dmlc/logging.h>
#include <gtest/gtest.h>
#include "txtvm/txtvm.h"

TEST(Tensor, Expr) {
    using namespace txtvm;
    Var m, n, k;
    Tensor A({m, k});
    Tensor B({n, k});

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "threadsafe";
    return RUN_ALL_TESTS();
}
