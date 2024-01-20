#ifndef TVM_TENSOR_H_
#define TVM_TENSOR_H_

#include "expr.h"

namespace tvm {

class Tensor {
private:
    Expr src_expr;
};

}; // end of namespace tvm


#endif // TVM_TENSOR_H_
