def multiply(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    if len(A[0]) != len(B):
        raise ValueError("Size mismatch")

    res: list[list[float]] = [
        [
            0. for _ in range(len(B[0]))
        ] for _ in range(len(A))
    ]

    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(B)):
                res[i][j] += A[i][k] * B[k][j]

    return res
