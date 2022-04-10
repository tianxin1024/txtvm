from txtvm import cpp as txtvm

def test_basic():
    a = txtvm.Var('a', 0);
    b = txtvm.Var('b', 0);
    z = txtvm.Var(a, b);
    assert txtvm.format_str(z) == 'max(%s, %s)' % (a.name, b.name);


if __name__ == "__main__":
    test_basic()
    
