#ifndef TVM_TENSOR_H_
#define TVM_TENSOR_H_

#include <string>
#include <vector>
#include "tvm/container.h"
#include "ir/FunctionBase.h"
#include <type_traits>

#include "base.h"
#include "expr.h"

namespace tvm {

class TensorNode;
class OperationNode;

using Halide::IR::FunctionRef;

class Tensor : public FunctionRef {
public:
    Tensor() {}

    explicit Tensor(std::shared_ptr<Node> n) : NodeRef(n) {}

    Tensor(Array<Expr> shape, FCompute fcompute, std::string name = "tensor");
    Tensor(Array<Expr> shape, std::function<Expr(Var)> f, std::string name = "tensor")
        : Tensor(shape, GetFCompute(f), name) {}
    Tensor(Array<Expr> shape, std::function<Expr(Var, Var)> f, std::string name = "tensor")
        : Tensor(shape, GetFCompute(f), name) {}
    Tensor(Array<Expr> shape, std::function<Expr(Var, Var, Var)> f, std::string name = "tensor")
        : Tensor(shape, GetFCompute(f), name) {}
    Tensor(Array<Expr> shape, std::function<Expr(Var, Var, Var, Var)> f, std::string name = "tensor")
        : Tensor(shape, GetFCompute(f), name) {}

    inline const TensorNode* operator->() const;

    inline size_t ndim() const;

    template<typename... Args>
    inline Expr operator()(Args&& ...args) const {
        Array<Expr> indices{std::forward<Args>(args)...};
        return operator()(indices);
    }

    Expr operator()(Array<Expr> indices) const;

    friend std::ostream& operator<<(std::ostream &os, const Tensor& t);
};


class TensorNode : public Node {
public:
    std::string name;
    DataType dtype;
    Array<Var> dim_index;
    Array<Expr> shape;
    Expr source;
    TensorNode() {}

    const char* type_key() const override {
        return "TensorNode";
    }

    void VisitAttrs(AttrVisitor* visitor) override {
        visitor->Visit("name", &name);
        visitor->Visit("dtype", &dtype);
        visitor->Visit("dim_var", &dim_var);
        visitor->Visit("shape", &shape);
        visitor->Visit("source", &source);
    }

};

// implementations
inline const TensorNode* Tensor::operator->() const {
    return static_cast<const TensorNode*>(node_.get());
}

inline size_t Tensor::ndim() const {
    return (*this)->shape.size();
}

inline std::ostream& operator<<(std::ostream &os, const Tensor& t) {
    os << "Tensor(shape=" << t->shape
        << ", source=" << t->source
        << ", name=" << t->name << ')';
    return os;
}


}; // end of namespace tvm


#endif // TVM_TENSOR_H_
