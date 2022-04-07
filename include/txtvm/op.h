#ifndef TXTVM_OP_H
#define TXTVM_OP_H

#include <string>
#include "./expr.h"

namespace txtvm {

    class BinaryOp {
    public:
        virtual std::string Format(const std::string& lhs, const std::string& rhs);
    }; // class end of BinaryOp

    class UnaryOp {
    public:

    }; // class end of UnaryOp

    class AddOp : public BinaryOp {
    public:
        static AddOp* Get();

    };  // class end of AddOp

    class SubOp : public BinaryOp {
    public:
        static SubOp* Get();

    };  // class end of SubOp

    class MulOp : public BinaryOp {
    public:
        static MulOp* Get();

    };  // class end of MulOp


    class DivOp : public BinaryOp {
    public:
        static DivOp* Get();

    };  // class end of DivOp


} // namespace txtvm


#endif  // TXTVM_OP_H
