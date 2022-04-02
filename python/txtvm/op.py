from __future__ import absolute_import as _abs

__binary_op_cls = None

class BinaryOp(object):
    """Base class of binary operator"""
    def __call__(self, lhs, rhs):
        return __binary_op_cls(self, lhs, rhs)

class AddOp(BinaryOp):
    pass

class SubOp(BinaryOp):
    pass

class MulOp(BinaryOp):
    pass

class DivOp(BinaryOp):
    pass

add = AddOp()
sub = SubOp()
mul = MulOp()
div = DivOp()
