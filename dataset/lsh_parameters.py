from math import log2, floor


def compute_r(J: float, b: int, p: float = 0.8) -> float:
    return log2(1 - (1 - p) ** (1 / b)) / log2(J)


def prob(J: float, r: int, b: int) -> float:
    return 1 - (1 - J**r) ** b


def jaccard_sim(r: int, b: int, p: float) -> float:
    return (1 - (1 - p) ** (1 / b)) ** (1 / r)



p = 0.9
print(jaccard_sim(2, 40, p))

exit(0)
print(f"p = {p}")
l = []
for r in range(2, 11):
    for b in range(2, 20):
        l.append((r, b, jaccard_sim(r, b, p)))

l.sort(key=lambda x: x[2])
for r, b, j in l:
    print(f"J = {j:.2f}, r = {r}, b = {b}")

print("\n=============================\n")
exit(0)

J = 0.2
print(f"J = {J}")
for b in range(50, 2001, 50):
    r = compute_r(J, b)
    print(
        f"b = {b}, r = {r}, b * {floor(r)} = {b * floor(r)}, ~p = {prob(J, floor(r), b)}"
    )
