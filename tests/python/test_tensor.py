import txtvm

def test_tensor():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    T = txtvm.Tensor(3, lambda i, j, k: A(i, k) * B(j, k),
                    shape=(A.shape[0], B.shape[0], A.shape[1]))
    print(txtvm.format_str(T.expr))

def test_tensor_inputs():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    T = txtvm.Tensor(3, lambda i, j, k: A(i, k) * B(j, k), 
                    shape=(A.shape[0], B.shape[0], A.shape[1]))
    assert(T.input_tensors() == [A, B])

def test_tensor_reduce():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    T = txtvm.Tensor(3, lambda i, j, k: A(i, k) * B(j, k),
                    shape=(A.shape[0], B.shape[0], A.shape[1]))
    rd = txtvm.RDom(txtvm.Range(A.shape[1]))
    C = txtvm.Tensor(2, lambda i, j: txtvm.reduce_sum(T(i, j, rd.index[0]), rdom=rd),
                    shape=(A.shape[0], B.shape[0]))
    print(txtvm.format_str(C.expr))

if __name__ == "__main__":
    test_tensor_inputs()
    test_tensor_reduce()
