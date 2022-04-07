import txtvm

def test_range_infer():
    x = txtvm.Var('x')
    y = txtvm.Var('y')
    t = txtvm.Var('t')
    z = x + y + t
    zr = txtvm.infer_range(z, {x: txtvm.Range(10, 20), y : txtvm.Range(10, 11)})
    assert str(zr) == "((20 + t0), (30 + t0))"
    print(str(zr))
    # TODO add 顺序相反
    print("--------")
    # assert str(zr) == "((t0 + 20), (t0 + 30))"


def test_tensor_dom_infer():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    rd = txtvm.RDom(txtvm.Range(A.shape[1]))

    T = txtvm.Tensor(2, lambda i, j: 
                    txtvm.reduce_sum(A(i, rd.index[0]) * B(j, rd.index[0]), rdom=rd),
                    shape=(A.shape[0], B.shape[0], A.shape[1]))
    C = txtvm.Tensor(2, lambda i, j: T(i, j),
                    shape=(A.shape[0], B.shape[0]))

    cdom = [txtvm.Range(0, 10), txtvm.Range(1, 11)]
    tdom = C.infer_input_domains(cdom, inputs=[T])[T]
    assert T.is_rtensor
    assert str(tdom[0]) == "(0, 10)"

if __name__ == "__main__":
    test_range_infer()
    test_tensor_dom_infer()
