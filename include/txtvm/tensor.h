#ifndef TVM_TENSOR_H_
#define TVM_TENSOR_H_

#include "expr.h"
#include "base.h"
#include "array.h"

namespace tvm {

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

    inline size_t ndim() const {
        return static_cast<const TensorNode*>(node_.get())->shape.size();
    }

    inline const std::string& name() const {
        return static_cast<const TensorNode*>(node_.get())->name;
    }

    inline DataType dtype() const {
        return static_cast<const TensorNode*>(node_.get())->dtype;
    }

    inline const Expr& source() const {
        return static_cast<const TensorNode*>(node_.get())->source;
    }

    inline const Array<Var>& dim_index() const {
        return static_cast<const TensorNode*>(node_.get())->dim_index;
    }

    inline const Array<Expr>& shape() const {
        return static_cast<const TensorNode*>(node_.get())->shape;
    }

    template<typename... Args>
    inline Expr operator()(Args&& ...args) const {
        Array<Expr> indices{std::forward<Args>(args)...};
        return operator()(indices);
    }

    Expr operator()(Array<Expr> indices) const;

    std::vector<Tensor> InputTensors() const;

    bool IsRTensor() const;

    friend std::ostream& operator<<(std::ostream &os, const Tensor& t) {
        os << "Tensor(shape=" << t.shape()
            << ", source=" << t.source()
            << ", name=" << t.name() << ')';
        return os;
    }
};

}; // end of namespace tvm


#endif // TVM_TENSOR_H_
