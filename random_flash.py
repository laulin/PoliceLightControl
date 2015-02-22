import random

univ = ("RED_MASK", "RED_MASK|BLUE_MASK", "BLUE_MASK", "NONE_MASK")

for i in range(32):
    line = []
    for j in range(8):
        line.append(random.choice(univ))

    print(",".join(line) + ",")
                    
