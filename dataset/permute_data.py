import random
import sys
import os


def kendall_tau_permutation(sequence: list, p: float) -> list:
    """
    The Kendall Tau distance is a metric that counts the number of pairwise disagreements between two sequenceuences.
    The alogirthm works as follows:
    1. given a probability `p`, define the number of inversions as `k = p * (n * (n - 1) / 2)`, where `n` is the length of the sequenceuence.
    2. choose `k` pairs of indices randomly, and swap the elements at those indices.
    3. return the permuted sequenceuence.

    :param sequence: the original sequenceuence
    :param p: the probability of introducing inversions
    :return: the permuted sequenceuence
    """

    n = len(sequence)
    k = round(p * (n * (n - 1) // 2))

    for q in range(k):

        # print progress on standard error
        if q % 100 == 0:
            print(f"\r{q}/{k}", file=sys.stderr, end="")

        i, j = random.sample(range(n), 2)
        sequence[i], sequence[j] = sequence[j], sequence[i]

    print(file=sys.stderr)
    return sequence


if len(sys.argv) < 3:
    print("Usage: python3 permute_data.py <file_name> <p>")
    exit(0)

FILE_NAME = sys.argv[1]
p = float(sys.argv[2])

f = open(FILE_NAME, "r")

n = int(f.readline())
m = int(f.readline())

edges = [tuple(map(int, f.readline().split())) for _ in range(m)]

f.close()

with open(f"data/{os.path.basename(FILE_NAME)}.perm", "w") as f:
    f.write(f"{n}\n{m}\n")
    # for u, v in kendall_tau_permutation(edges, p):
    #     f.write(f"{u} {v}\n")
    random.shuffle(edges)
    for u, v in edges:
        f.write(f"{u} {v}\n")