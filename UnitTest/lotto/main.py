import random


# Lottto Jackpot generator

results1 = [0, 0, 0, 0, 0]
results2 = [0, 0]

results1 = random.sample(range(1, 51, 1), 5)
results2 = random.sample(range(1, 13, 1), 2)

print(results1)
print(results2)














