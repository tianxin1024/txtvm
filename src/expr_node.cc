#include <txtvm/base.h>
#include <txtvm/expr_node.h>
#include <memory>

namespace tvm {

TVM_REGISTER_NODE_TYPE(VarNode);
TVM_REGISTER_NODE_TYPE(IntNode);
TVM_REGISTER_NODE_TYPE(FloatNode);
TVM_REGISTER_NODE_TYPE(UnaryOpNode);
TVM_REGISTER_NODE_TYPE(BinaryOpNode);

} // end of namespace tvm
