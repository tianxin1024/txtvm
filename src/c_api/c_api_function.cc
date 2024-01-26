#include "tvm/expr.h"
#include "c_api_registry.h"

namespace dmlc {
DMLC_REGISTRY_ENABLE(::tvm::APIFunctionReg);
}; // end of namespace dmlc

namespace tvm {

using ArgStack = const std::vector<APIVariantValue>;
using RetValue = APIVariantValue;


}; // end of namespace tvm
