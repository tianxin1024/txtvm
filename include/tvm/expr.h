#ifndef TVM_EXPR_H_
#define TVM_EXPR_H_

#include <type_traits>
#include "base.h"
#include "ir/Expr.h"
#include "ir/IROperator.h"

namespace tvm {

using namespace Halide::Internal;

using Halide::Type;
using Halide::Expr;
using Halide::Int;
using Halide::UInt;
using Halide::Handle;

// functions
using Halide::cast;
using Halide::min;
using Halide::max;
using Halide::abs;
using Halide::select;

using Halide::Expr;
using Var = Halide::VarExpr;

}; // end of namespace tvm


#endif // TVM_EXPR_H_
