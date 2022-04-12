from txtvm import cpp as txtvm

def test_basic():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    z = a + b
    assert txtvm.format_str(z) == '(%s + %s)' % (a.name, b.name)


if __name__ == "__main__":
    test_basic()
    
