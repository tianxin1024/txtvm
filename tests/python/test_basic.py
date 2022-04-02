import txtvm
from txtvm import expr



def test_bind():
    x = txtvm.Var('x')
    y = x + 1
    z = txtvm.bind(y, {x : txtvm.const(10) + 9})
    assert txtvm.format_str(z) == '((10 + 9) + 1)'

def test_basic():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    c = a + b
    assert txtvm.format_str(c) == '(%s + %s)' % (a.name, b.name)


if __name__ == "__main__":
    test_basic()
    test_bind()
