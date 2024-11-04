from typing import Tuple


def search(arr: list[int], x: int) -> int:
    for i in range(len(arr)):
        if arr[i] == x:
            return i
    return -1


def search_cmp_profile(arr: list[int], x: int) -> Tuple[int, int]:
    cmps = 0
    for i in range(len(arr)):
        cmps += 1
        if arr[i] == x:
            return i, cmps

    return -1, cmps
