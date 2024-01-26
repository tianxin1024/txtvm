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


_init_function_module("txtvm")
