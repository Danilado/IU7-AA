from .fill_table import fill_table


def levenstein_iter(s1: str, s2: str) -> int:
    table = fill_table(s1, s2)

    for i in range(1, len(table)):
        for j in range(1, len(table[0])):
            table[i][j] = min(
                table[i-1][j] + 1,
                table[i][j-1] + 1,
                table[i-1][j-1] + (int(s1[i-1] != s2[j-1]))
            )

    return table[-1][-1]
