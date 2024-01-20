#include <txtvm/expr_node.h>
#include <memory>

namespace tvm {

TVM_REGISTER_NODE_TYPE(VarNode);
TVM_REGISTER_NODE_TYPE(IntNode);
TVM_REGISTER_NODE_TYPE(FloatNode);
TVM_REGISTER_NODE_TYPE(UnaryNode);
TVM_REGISTER_NODE_TYPE(BinaryNode);

} // end of namespace tvm
