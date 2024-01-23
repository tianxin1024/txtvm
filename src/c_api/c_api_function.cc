#include <txtvm/expr.h>
#include <txtvm/op.h>
#include "c_api_registry.h"

namespace dmlc {
DMLC_REGISTRY_ENABLE(::tvm::APIFunctionReg);
}; // end of namespace dmlc

namespace tvm {

using ArgStack = const std::vector<APIVariantValue>;
using RetValue = APIVariantValue;

TVM_REGISTER_API(_Var)
.set_body([](const ArgStack& args, RetValue *ret) {
        *ret = Var(args.at(0), static_cast<DataType>(static_cast<int>(args.at(1))));
        })
.add_argument("name", "str", "name of the var")
.add_argument("dtype", "int", "data type of var");


TVM_REGISTER_API(constant)
.set_body([](const ArgStack& args, RetValue *ret) {
        if (args.at(0).type_id == kLong) {
            *ret = IntConstant(args.at(0));
        } else if (args.at(0).type_id == kDouble) {
            *ret = FloatConstant(args.at(0));
        } else {
            LOG(FATAL) << "only accept int or float";
        }
})
.add_argument("src", "Number", "source number");


TVM_REGISTER_API(_binary_op)
.set_body([](const ArgStack& args, RetValue *ret) {
        CHECK(args.at(0).type_id == kStr);
        *ret = (*BinaryOp::Get(args.at(0).str.c_str()))(args.at(1), args.at(2));
})
.add_argument("op", "str", "operator")
.add_argument("lhs", "Expr", "left operand")
.add_argument("rhs", "Expr", "right operand");


TVM_REGISTER_API(format_str)
.set_body([](const ArgStack& args, RetValue *ret) {
        std::ostringstream os;
        os << Expr(args.at(0));
        *ret = os.str();
})
.add_argument("expr", "Expr", "expression to be printed");

}; // end of namespace tvm
