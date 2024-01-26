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

def _symbol(value):
    """Convert a value to expression."""
    if isinstance(value, _Number):
        return const(value)
    elif isinstance(value, list):
        value = [_symbol(x) for x in value]
        return _function_internal._Array(*value)
    else:
        return value

_init_function_module("txtvm")
