#ifndef TVM_TENSOR_H_
#define TVM_TENSOR_H_

#include "expr.h"
#include "base.h"
#include "array.h"

namespace tvm {

class TensorNode;

using FCompute = std::function<Expr (const Array<Var>& i)>;

inline FCompute GetFCompute(std::function<Expr(Var x)> f) {
    return [f] (const Array<Var>& i) { return f(i[0]); };
}
inline FCompute GetFCompute(std::function<Expr(Var, Var)> f) {
    return [f] (const Array<Var>& i) { return f(i[0], i[1]); };
}
inline FCompute GetFCompute(std::function<Expr(Var, Var, Var)> f) {
    return [f] (const Array<Var>& i) { return f(i[0], i[1], i[2]); };
}
inline FCompute GetFCompute(std::function<Expr(Var, Var, Var, Var)> f) {
    return [f] (const Array<Var>& i) { return f(i[0], i[1], i[2], i[3]); };
}

class Tensor : public NodeRef {
public:
    Tensor() {}

    explicit Tensor(Array<Expr> shape, 
                    std::string name = "tensor",
                    DataType dtype = kFloat32);

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

    std::vector<Tensor> InputTensors() const;

    bool IsRTensor() const;

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
    }

    void VisitNodeRefFields(FNodeRefVisit fvisit) override {
        fvisit("dim_index", &dim_index);
        fvisit("shape", &shape);
        fvisit("source", &source);
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
