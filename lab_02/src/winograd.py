def multiply(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    if len(A[0]) != len(B):
        raise ValueError("Size mismatch")
    n: int = len(B)

    res: list[list[float]] = [
        [
            0. for _ in range(len(B[0]))
        ] for _ in range(len(A))
    ]

    Harr = [0. for _ in range(len(A))]
    Varr = [0. for _ in range(len(B[0]))]

    for i in range(len(A)):
        for j in range(n // 2):
            Harr[i] = Harr[i] + A[i][2*j] * A[i][2*j + 1]

    for i in range(len(B[0])):
        for j in range(n // 2):
            Varr[i] = Varr[i] + B[2*j][i] * B[2*j + 1][i]

    for i in range(len(A)):
        for j in range(len(B[0])):
            res[i][j] = -Harr[i] - Varr[j]
            for k in range(n // 2):
                res[i][j] = res[i][j] + (A[i][2*k]+B[2*k + 1][j]) * \
                    (A[i][2*k + 1]+B[2*k][j])
            if (n % 2 != 0):
                res[i][j] = res[i][j] + A[i][-1] * B[-1][j]

    return res


def multiply_optimized(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    if len(A[0]) != len(B):
        raise ValueError("Size mismatch")
    n: int = len(B)

    res: list[list[float]] = [
        [
            0. for _ in range(len(B[0]))
        ] for _ in range(len(A))
    ]

    Harr: list[float] = [0. for _ in range(len(A))]
    Varr: list[float] = [0. for _ in range(len(B[0]))]

    for j in range(n // 2):
        Harr[0] = Harr[0] + A[0][2*j] * A[0][2*j + 1]

    for i in range(1, len(A)):
        for j in range(n // 2):
            Harr[i] = Harr[i] + A[i][2*j] * A[i][2*j + 1]

    #

    for j in range(n // 2):
        Varr[0] = Varr[0] + B[2*j][0] * B[2*j + 1][0]

    for i in range(1, len(B[0])):
        for j in range(n // 2):
            Varr[i] = Varr[i] + B[2*j][i] * B[2*j + 1][i]

    #

    for j in range(len(B[0])):
        res[0][j] = -Harr[0] - Varr[j]
        for k in range(0, n - 1, 2):
            res[0][j] = res[0][j] + (A[0][k] + B[k+1][j]) * \
                (A[0][k+1] + B[k][j])
        if n % 2 != 0:
            res[0][j] += A[0][-1] * B[-1][j]

    for i in range(1, len(A)):
        for j in range(len(B[0])):
            res[i][j] = -Harr[i] - Varr[j]
            for k in range(0, n - 1, 2):
                res[i][j] = res[i][j] + (A[i][k] + B[k+1][j]) * \
                    (A[i][k+1] + B[k][j])
            if n % 2 != 0:
                res[i][j] = res[i][j] + A[i][-1] * B[-1][j]

    return res
