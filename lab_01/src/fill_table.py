def fill_table(s1: str, s2: str) -> list[list[int]]:
    table = [
        [0] * (len(s2) + 1)
        for _ in range(len(s1) + 1)
    ]

    for i in range(len(table)):
        table[i][0] = i

    for i in range(len(table[0])):
        table[0][i] = i

    return table
