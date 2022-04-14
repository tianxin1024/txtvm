from txtvm import cpp as txtvm

def test_basic():
    a = txtvm.Var('a')
    b = txtvm.Var('b')
    import ipdb
    ipdb.set_trace()
    c = a + b

    assert a == c.lhs
    assert c.dtype == txtvm.int32
    assert txtvm.format_str(c) == '(%s + %s)' % (a.name, b.name)


def test_array():
    a = txtvm.Var('a')
    x = txtvm.function._symbol([1,2,a])
    print(type(x))
    print(len(x))
    print(x[4])


if __name__ == "__main__":
    test_basic()
    test_array()
    
