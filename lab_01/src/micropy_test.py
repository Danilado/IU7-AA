import urandom
import utime


def generate_random_string(n):
    # Определяем набор символов (буквы и цифры)
    characters = list(
        "ABCDEFGHIJKLMNOPQRSTUVAdnakjndkajsndjbfkse12343295014926037951")
    # Генерируем случайную строку
    random_string = ''.join(
        characters[urandom.getrandbits(6) % len(characters)] for _ in range(n))
    return random_string


def levenstein_rec(s1: str, s2: str) -> int:
    if len(s1) == 0:
        return len(s2)

    if len(s2) == 0:
        return len(s1)

    if s1[0] == s2[0]:
        return levenstein_rec(s1[1:], s2[1:])

    return 1 + min(
        levenstein_rec(s1[1:], s2),
        levenstein_rec(s1, s2[1:]),
        levenstein_rec(s1[1:], s2[1:])
    )


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


for i in range(1, 10):
    s1 = generate_random_string(i)
    s2 = generate_random_string(i)

    print(f"Length {i}: ", end="")

    stamp = utime.ticks_ms()
    levenstein_rec(s1, s2)
    stamp2 = utime.ticks_ms()

    print(f"rec - {stamp2-stamp}ms ", end="\t")

    stamp = utime.ticks_ms()
    levenstein_iter(s1, s2)
    stamp2 = utime.ticks_ms()

    print(f"iter - {stamp2-stamp}ms ", end="\t")

    stamp = utime.ticks_ms()
    damerau_levenstein(s1, s2)
    stamp2 = utime.ticks_ms()

    print(f"damerau - {stamp2-stamp}ms")
