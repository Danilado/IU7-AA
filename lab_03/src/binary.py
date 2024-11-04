from typing import Tuple


def binsearch(arr: list[int], x: int) -> int:
    left = 0
    right = len(arr) - 1

    while left <= right:
        c = (right + left) // 2
        if arr[c] == x:
            return c
        elif arr[c] < x:
            left = c + 1
        else:
            right = c - 1

    return -1


def binsearch_cmp_profile(arr: list[int], x: int) -> Tuple[int, int]:
    left = 0
    right = len(arr) - 1
    cmps = 0

    while left <= right:
        c = (right + left) // 2

        cmps += 1
        if arr[c] == x:
            return c, cmps
        elif arr[c] < x:
            left = c + 1
        else:
            right = c - 1
        cmps += 1

    return -1, cmps


def searchcopy(arr: list[int], x: int) -> int:
    tmparr: list[int] = sorted(arr)

    return binsearch(tmparr, x)


def searchsort(arr: list[int], x: int) -> int:
    arr = sorted(arr)

    return binsearch(arr, x)
