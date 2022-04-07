import txtvm


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

def test_simplify():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    e1 = a * (2 + 1) + b * 1
    e2 = a * (2 + 1) - b * 1
    e3 = txtvm.max(a * 3.3 + 5, 3 + 3.3 * a)
    e4 = a - a
    assert txtvm.format_str(txtvm.simplify(e1)) == '((%s * 3) + %s)' % (a.name, b.name)
    assert txtvm.format_str(txtvm.simplify(e2)) == '((%s * 3) + (%s * -1))' % (a.name, b.name)
    assert txtvm.format_str(txtvm.simplify(e3)) == '((%s * 3.3) + 5)' % (a.name)
    assert txtvm.format_str(txtvm.simplify(e4)) == '0'


if __name__ == "__main__":
    test_basic()
    test_bind()
    test_simplify()
