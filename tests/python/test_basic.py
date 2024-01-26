import txtvm
from txtvm import expr

def test_const():
    x = txtvm.const(1)
    assert x.type == 'int32'
    assert isinstance(x, txtvm.expr.IntImm)

if __name__ == "__main__":
    test_const()
