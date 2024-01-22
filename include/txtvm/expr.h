#ifndef TVM_EXPR_H_
#define TVM_EXPR_H_

#include <type_traits>
#include "base.h"

namespace tvm {

// forward declare Expr
class Expr;

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type >
inline Expr constant(T value);


class Expr : public NodeRef {
public:
    Expr() = default;
    Expr(const Expr& other) = default;
    Expr(Expr&& other) = default;
    Expr& operator=(const Expr& other) = default;
    Expr& operator=(Expr&& other) = default;

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type >
    Expr(T value) {
        *this = std::move(constant<T>(value));
    }

    explicit Expr(std::shared_ptr<Node> nptr) : NodeRef(std::move(nptr)) {
        CHECK(node_.get() != nullptr);
    }

    inline DataType dtype() const;
    
    friend std::ostream& operator<<(std::ostream &os, const Expr& e) {
        e.Print(os);
        return os;
    }

private:
    void Print(std::ostream& os) const;
};


class Var : public Expr {
public:
    Var(std::string name="", DataType dtype=kInt32);
};

Expr IntConstant(int64_t value);
Expr FloatConstant(int64_t value);

class ExprNode : public Node {
public:
    DataType dtype_{kUnknown};
};

inline DataType Expr::dtype() const {
    return static_cast<const ExprNode*>(node_.get())->dtype_;
}

template<typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type >
inline Expr constant(T value) {
    if (std::is_integral<T>::value) {
        return IntConstant(static_cast<int64_t>(value));
    } else {
        return FloatConstant(static_cast<double>(value));
    }
}

}; // end of namespace tvm



#endif // TVM_EXPR_H_
