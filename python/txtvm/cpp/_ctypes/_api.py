#coding: utf-8
"""Symbolic configuration API."""
from __future__ import absolute_import as _abs

import ctypes
import sys
from numbers import Number as Number

from .._base import _LIB
from .._base import c_str, py_str, string_types
from .._base import FunctionHandle, NodeHandle
from .._base import check_all, ctypes2docstring

class ArgVariant(ctypes.Union):
    _fields_ = [("v_long", ctypes.c_long),
                ("v_double", ctypes.c_double),
                ("v_str", ctypes.c_char_p),
                ("v_handle", ctypes.c_void_p)]

kNull       = 0
kLong       = 1
kDouble     = 2
kStr        = 3
kNodeHandle = 4

RET_SWITCH = None

class NodeBase(object):
    """Symbol is symbolic graph."""
    __slots__ = ["handle"]
    def __init__(self, handle):
        """Initialize the function with handle

        Parameters
        ----------
        handle : SymbolHandle
            the handle to the underlying C++ Symbol
        """
        self.handle = handle

    def __del__(self):
        check_all(_LIB.TVMNodeFree(self.handle))

    def __getattr__(self, name):
        ret_val = ArgVariant()
        ret_typeid = ctypes.c_int()
        check_all(_LIB.TVMNodeGetAttr(
            self.handle, c_str(name),
            ctypes.byref(ret_val), ctypes.byref(ret_typeid)))
        return RET_SWITCH[ret_typeid.value](ret_val)
