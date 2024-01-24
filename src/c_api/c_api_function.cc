#include "txtvm/expr.h"
#include "txtvm/op.h"
#include "txtvm/tensor.h"
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


TVM_REGISTER_API(_raw_ptr)
.set_body([](const ArgStack& args, RetValue *ret) {
        CHECK(args.at(0).type_id == kNodeHandle);
        *ret = reinterpret_cast<int64_t>(args.at(0).sptr.get());
})
.add_argument("src", "NodeBase", "the node base");


TVM_REGISTER_API(_Range)
.set_body([](const ArgStack& args, RetValue *ret) {
    *ret = Range(args.at(0), args.at(1));
})
.add_argument("begin", "Expr", "beginning of the range.")
.add_argument("end", "Expr", "end of the range");


TVM_REGISTER_API(_Array)
.set_body([](const ArgStack& args, RetValue *ret) {
    std::vector<std::shared_ptr<Node>> data;
    for (size_t i = 0; i < args.size(); ++i) {
        CHECK(args.at(i).type_id == kNodeHandle);
        data.push_back(args.at(i).sptr);
    }
    auto node = std::make_shared<ArrayNode>();
    node->data = std::move(data);
    ret->type_id = kNodeHandle;
    ret->sptr = node;
});


TVM_REGISTER_API(_ArrayGetItem)
.set_body([](const ArgStack& args, RetValue *ret) {
    CHECK(args.at(0).type_id == kNodeHandle);
    int64_t i = args.at(1);
    auto& sptr = args.at(0).sptr;
    CHECK(sptr->is_type<ArrayNode>());
    auto* n = static_cast<const ArrayNode*>(sptr.get());
    CHECK_LT(static_cast<size_t>(i), n->data.size())
        << "out of bound of array";
    ret->sptr = n->data[i];
    ret->type_id = kNodeHandle;
});


TVM_REGISTER_API(_ArraySize)
.set_body([](const ArgStack& args,  RetValue *ret) {
    CHECK(args.at(0).type_id == kNodeHandle);
    auto& sptr = args.at(0).sptr;
    CHECK(sptr->is_type<ArrayNode>());
    *ret = static_cast<int64_t>(
        static_cast<const ArrayNode*>(sptr.get())->data.size());
});


TVM_REGISTER_API(_TensorInput)
.set_body([](const ArgStack& args, RetValue *ret) {
    *ret = Tensor(
        static_cast<Array<Expr>>(args.at(0)),
        static_cast<std::string>(args.at(1)),
        static_cast<DataType>(static_cast<int>(args.at(1))));
});


TVM_REGISTER_API(format_str)
.set_body([](const ArgStack& args, RetValue *ret) {
    CHECK(args.at(0).type_id == kNodeHandle);
    std::ostringstream os;
    auto& sptr = args.at(0).sptr;
    if (sptr->is_type<TensorNode>()) {
        os << args.at(0).operator Tensor();
    } else if (sptr->is_type<RDomainNode>()) {
        os << args.at(0).operator RDomain();
    } else if (sptr->is_type<RangeNode>()) {
        os << args.at(0).operator Range();
    } else {
        os << args.at(0).operator Expr();
    }
    *ret = os.str();
})
.add_argument("expr", "Expr", "expression to be printed");

}; // end of namespace tvm
