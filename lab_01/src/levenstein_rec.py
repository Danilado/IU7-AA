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
