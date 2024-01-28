import txtvm

def test_tensor():
    m = txtvm.Var('m')
    n = txtvm.Var('n')
    l = txtvm.Var('l')
    A = txtvm.Tensor((m, l), name='A')
    B = txtvm.Tensor((m, l), name='B')
    T = txtvm.Tensor((m, n, l), lambda i, j, k: A(i, k) * B(j, k))
    print(txtvm.format_str(T.source))
    assert(tuple(T.shape) == (m, n, l))
    assert(A.source is None)


if __name__ == "__main__":
    test_tensor()
