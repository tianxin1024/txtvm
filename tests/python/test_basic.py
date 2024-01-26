import txtvm
from txtvm import expr

def test_const():
    x = txtvm.const(1)
    assert x.type == 'int32'
    assert isinstance(x, txtvm.expr.IntImm)

def test_make():
    x = txtvm.const(1)
    y = txtvm.make.IntImm('int32', 1)
    z = x + y
    print(txtvm.format_str(z))

def test_ir():
    x = txtvm.const(1)
    y = txtvm.make.IntImm('int32', 1)
    z = x + y
    stmt = txtvm.make.Evaluate(z)
    assert isinstance(stmt, txtvm.stmt.Evaluate)
    print(txtvm.format_str(stmt))

if __name__ == "__main__":
    test_const()
    test_make()
    test_ir()
