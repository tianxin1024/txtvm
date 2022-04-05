import txtvm

def test_tensor():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    T = txtvm.Tensor(3, lambda i, j, k: A(i, k) * B(j, k))
    print(txtvm.format_str(T.expr))


if __name__ == "__main__":
    test_tensor()
