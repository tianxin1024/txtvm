from txtvm import cpp as txtvm

def test_basic():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    c = a + b


    assert c == c.lhs
    assert c.dtype == txtvm.int32
    assert txtvm.format_str(c) == '(%s + %s)' % (a.name, b.name)


if __name__ == "__main__":
    test_basic()
    
