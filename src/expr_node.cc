#include "txtvm/expr_node.h"
#include <memory>

namespace txtvm {

    TXTVM_REGISTER_NODE_TYPE(VarNode);
    TXTVM_REGISTER_NODE_TYPE(IntNode);
    TXTVM_REGISTER_NODE_TYPE(FloatNode);
    TXTVM_REGISTER_NODE_TYPE(UnaryNode);
    TXTVM_REGISTER_NODE_TYPE(BinaryNode);

} // namespace txtvm
