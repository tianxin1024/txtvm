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

def test_basic():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    c = a + b
    assert txtvm.format_str(c) == '(%s + %s)' % (a.name, b.name)

def test_array():
    a = txtvm.convert([1, 2, 3])

def test_stmt():
    print(txtvm.make.Provide('a', [1, 2, 3], [1, 2, 3]))
    print(txtvm.make.For('a', 0, 1,
                         txtvm.stmt.For.Serial, 0,
                         txtvm.make.Evaluate(0)))

if __name__ == "__main__":
    test_const()
    test_make()
    test_ir()
