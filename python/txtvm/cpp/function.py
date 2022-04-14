from __future__ import absolute_import as _abs
from numbers import Number as _Number
from ._ctypes._api import _init_function_module
from . import _function_internal

int32 = 1
float32 = 2

def Var(name="tindex", dtype=int32):
    """Create a new variable with specified name and dtype

        Parameters
        ----------
        name : str
            The name

        dtype : int
            The data type
    """
    return _function_internal._Var(name, dtype);


def _symbol(value):
    """Convert a value to expression."""
    if isinstance(value, _Number):
        return constant(value)
    elif isinstance(value, list):
        value = [_symbol(x) for x in value]
        return _function_internal._Array(*value)
    else:
        return value


def max(lhs, rhs):
    """Max of two expressions

        Parameters
        ----------
        lhs : Expr/number
            The left operand

        rhs : Expr/number
            The right operand
    """
    return binary_op("max", lhs, rhs)

def min(lhs, rhs):
    """Min of two expressions

        Parameters
        ----------
        lhs : Expr/number
            The left operand

        rhs : Expr/number
            The right operand
    """
    return binary_op("min", lhs, rhs)

_init_function_module("txtvm.cpp")
