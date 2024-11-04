import simple
import winograd
from typing import Callable


TEST_MATRICES = [
    ([
        [1, 2],
        [3, 4],
        [5, 6]
    ], [
        [7, 8, 9],
        [0, 1, 2]
    ]),
    ([
        [1, 2],
        [3, 4]
    ], [
        [5, 6],
        [7, 8],
    ]),
    (
        [
            [1, 2]
        ], [
            [3, 4]
        ]
    )
]
TEST_TARGETS = [
    [
        [7, 10, 13],
        [21, 28, 35],
        [35, 46, 57]
    ],
    [
        [19, 22],
        [43, 50]
    ],
    None
]


def test(func: Callable) -> int:
    fails: int = 0
    for i in range(len(TEST_MATRICES)):
        try:
            res = func(TEST_MATRICES[i][0], TEST_MATRICES[i][1])
            if res != TEST_TARGETS[i]:
                fails += 1
                print("fail: expected", TEST_TARGETS[i], " got ", res)
        except ValueError:
            if (TEST_TARGETS[i] != None):
                fails += 1

    return fails


def test_simple() -> int:
    return test(simple.multiply)


def test_winograd() -> int:
    return test(winograd.multiply)


def test_winograd_opt() -> int:
    return test(winograd.multiply_optimized)


def main() -> int:
    res: int = 0

    print("Testing simple multiplication...")
    tmp = test_simple()
    print(f"Simple multiplication finished {
          f'with {tmp} errors' if tmp else 'without errors'}")
    res += tmp

    print("Testing winograd multiplication...")
    tmp = test_winograd()
    print(f"Winograd multiplication finished {
          f'with {tmp} errors' if tmp else 'without errors'}")
    res += tmp

    print("Testing optimized winograd multiplication...")
    tmp = test_winograd_opt()
    print(f"Optimized Winograd multiplication finished {
          f'with {tmp} errors' if tmp else 'without errors'}")
    res += tmp

    return res


if __name__ == "__main__":
    rc = main()
    exit(rc)
