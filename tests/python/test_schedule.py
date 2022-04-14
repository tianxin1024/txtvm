import txtvm


def test_schedule():
    A = txtvm.Tensor(2, name='A')
    B = txtvm.Tensor(2, name='B')
    rd = txtvm.RDom(txtvm.Range(A.shape[1]))
    T = txtvm.Tensor(2, lambda i, j :
                    txtvm.reduce_sum(A(i, rd.index[0]) * B(j, rd.index[0]), rdom=rd),
                    shape=(A.shape[0], B.shape[0]), name='T')
    C = txtvm.Tensor(2, lambda i, j: T(i, j),
                    shape=(A.shape[0], B.shape[0]), name='C')
    bufA = txtvm.Buffer(txtvm.Scope.Thread, name='A')
    bufB = txtvm.Buffer(txtvm.Scope.Thread, name='B')
    bufT = txtvm.Buffer(txtvm.Scope.Thread, name='T')

    schA = txtvm.Schedule(A, buffer=bufA)
    schB = txtvm.Schedule(B, buffer=bufB)
    schT = txtvm.Schedule(T, buffer=bufT)
    schC = txtvm.Schedule(C)

    Cx0 = txtvm.Split(dim=0, factor=64)
    Cy0 = txtvm.Split(dim=1, factor=64)
    Cx1 = txtvm.Split(dim=0, factor=8)
    Cy1 = txtvm.Split(dim=1, factor=8)
    Tk = txtvm.Split(dim=0, factor=8, rdom=True)

    schC.add_split(Cx0)
    schC.add_split(Cy0)
    schC.add_split(Cx1)
    schC.add_split(Cy1)
    schT.add_split(Tk)
    schC.attach(Cy1, schT)
    schT.attach(Tk, schA)
    schT.attach(Tk, schB)

    body = schC.realize()
    print('\n'.join(body))


if __name__ == "__main__":
    test_schedule()
