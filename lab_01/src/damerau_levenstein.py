from .fill_table import fill_table


def damerau_levenstein(s1: str, s2: str) -> int:
    table = fill_table(s1, s2)

    for i in range(1, len(table)):
        for j in range(1, len(table[0])):
            # check if swap is possible
            if i >= 2 and j >= 2 and s1[i-1] == s2[j-2] and s1[i-2] == s2[j-1]:
                table[i][j] = min(
                    table[i-1][j] + 1,
                    table[i][j-1] + 1,
                    table[i-1][j-1] + (int(s1[i-1] != s2[j-1])),
                    table[i-2][j-2] + 1  # swap
                )
            else:
                table[i][j] = min(
                    table[i-1][j] + 1,
                    table[i][j-1] + 1,
                    table[i-1][j-1] + (int(s1[i-1] != s2[j-1]))
                )

    return table[-1][-1]
