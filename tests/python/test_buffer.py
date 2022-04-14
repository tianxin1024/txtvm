import txtvm

def test_buffer():
    buf = txtvm.Buffer(txtvm.Scope.Thread)
    shape = [32, 16]
    domain = [txtvm.Range(v) for v in shape]
    buf.reshape(domain)
    x = txtvm.Var('x')
    y = txtvm.Var('y')
    assert txtvm.format_str(buf(y, x)) == '%s[(%s + (%s + %s))]' % (buf.name, x.name, y.name, shape[1])

if __name__ == "__main__":
    test_buffer()
