#ifndef TVM_BASE_H_
#define TVM_BASE_H_

#include <dmlc/logging.h>
#include <dmlc/registry.h>
#include <string>
#include <memory>
#include <functional>
#include <typeinfo>
#include <type_traits>
#include <tvm/node.h>

namespace tvm {

using ::tvm::Node;
using ::tvm::NodeRef;
using ::tvm::AttrVisitor;

using NodeFactory = std::function<std::shared_ptr<Node> ()>;

struct NodeFactoryReg 
    : public dmlc::FunctionRegEntryBase<NodeFactoryReg, NodeFactory> {
};


#define TVM_REGISTER_NODE_TYPE(TypeName)                                    \
    DMLC_REGISTRY_REGISTER(::tvm::NodeFactoryReg, NodeFactoryReg, TypeName) \
    .set_body([]() { return std::make_shared<TypeName>(); })

} // end of namespace tvm

#endif // TVM_BASE_H_
