from __future__ import absolute_import as _abs
from ._ctypes._api import _init_function_module
from numbers import Number as _Number, Integral as _Integral
from . import _function_internal
from . import make as _make

int32 = 1
float32 = 2

def const(value, dtype=None):

    if dtype is None:
        if isinstance(value, _Integral):
            dtype = 'int32'
        else:
            dtype = 'float32'
    return _function_internal._const(value, dtype)


def Var(name="tindex", dtype=int32):
    """Create a new variable with specified name and dtype

    Parameters
    ----------
    name : str
        The name

    dtype : int
        The data type
    """
    return _function_internal._Var(name, dtype)


def convert(value):
    """Convert a value to expression."""
    if isinstance(value, _Number):
        return const(value)
    elif isinstance(value, list):
        value = [convert(x) for x in value]
        return _function_internal._Array(*value)
    else:
        return value

def Range(begin, **kwargs):
    """Create a TVM Range object.

    User can either call:
    Range(10) to get a range in [0, 10)

    or 
    Range(begin=1, extent=10), to get a range in [0, 11)

    Parameters
    ----------
    begin : Expr
        The beginning of the expression
    """
    if "extent" in kwargs:
        return _function_internal._Range(begin, kwargs["extent"])
    else:
        return _function_internal._Range(0, begin)


_init_function_module("txtvm")
