from binary import binsearch
from simple import search

N = 1022


def test_simple():
    fails = 0

    arr = [i for i in range(N)]

    res = search(arr, -1)
    if (res != -1):
        fails += 1

    res = search([], 0)
    if (res != -1):
        fails += 1

    for i in range(N):
        res = search(arr, i)
        if res != i:
            fails += 1

    return fails


def test_binary():
    fails = 0

    arr = [i for i in range(N)]

    res = binsearch(arr, -1)
    if (res != -1):
        fails += 1

    res = binsearch([], 0)
    if (res != -1):
        fails += 1

    for i in range(N):
        res = binsearch(arr, i)
        if res != i:
            fails += 1

    return fails


def main():
    errors = 0
    errors += test_simple()
    errors += test_binary()

    print(f"Finished with {errors} errors")

    return errors


if __name__ == "__main__":
    rc = main()

    if rc:
        exit(1)

    exit(0)
