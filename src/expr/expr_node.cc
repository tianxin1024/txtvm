#include "txtvm/expr_node.h"
#include <memory>

namespace dmlc {
    DMLC_REGISTRY_ENABLE(::txtvm::NodeFactoryReg);
} // namespace dmlc


namespace txtvm {

    void Node::Destroy() {
        bool safe = true;
        this->VisitNodeRefFields([&safe](const char* k, NodeRef* r) {
                if (r->node_.get() != nullptr) safe = false;
        });

        if (!safe) {
            // explicit deletion via DFS
            // this is used to avoid stackoverflow caused by chain of deletions
            std::vector<Node*> stack{this};
            std::vector<std::shared_ptr<Node> > to_delete;
            while (!stack.empty()) {
                Node* n = stack.back();
                stack.pop_back();
                n->VisitNodeRefFields([&safe, &stack, &to_delete](const char* k, NodeRef* r) {
                    if (r->node_.unique()) {
                        stack.push_back(r->node_.get());
                        to_delete.emplace_back(std::move(r->node_));
                    } else {
                        r->node_.reset();
                    }
                });
            }
        }
    }

    TXTVM_REGISTER_NODE_TYPE(VarNode);
    TXTVM_REGISTER_NODE_TYPE(IntNode);
    TXTVM_REGISTER_NODE_TYPE(FloatNode);
    TXTVM_REGISTER_NODE_TYPE(UnaryOpNode);
    TXTVM_REGISTER_NODE_TYPE(BinaryOpNode);

} // namespace txtvm
