#!/usr/bin/env python3

import sys
import re
from math import sqrt, ceil, floor
from itertools import takewhile, product, chain

m = re.match(r'^target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+)', sys.stdin.readline())
x_min = int(m.group(1))
x_max = int(m.group(2))
y_min = int(m.group(3))
y_max = int(m.group(4))

rmin = sqrt(0.25 + 2 * x_min)
rmax = sqrt(0.25 + 2 * x_max)
lower = rmin - 0.5
upper = rmax - 0.5
# all positive ints between lower and upper are valid v_x if t ≥ v_x
joker_v_xs = range(int(ceil(lower)), int(floor(upper)) + 1)

valid_vv = set()

# max number of steps: - y_min * 2
for t in range(1, - y_min * 2 + 1):
    # print(f't = {t}, ', end='')
    # find valid v_y
    v_y_min = y_min / t + t / 2 - 0.5
    v_y_max = y_max / t + t / 2 - 0.5

    valid_v_y = range(int(ceil(v_y_min)), int(floor(v_y_max)) + 1)

    # find valid v_x
    v_x_min = max(x_min / t + t / 2 - 0.5, t)
    v_x_max = x_max / t + t / 2 - 0.5
    valid_v_x = chain(range(int(ceil(v_x_min)), int(floor(v_x_max)) + 1),
                    takewhile(lambda v: v <= t, joker_v_xs))

    valid_vv.update(product(valid_v_x, valid_v_y))


# print(counter)
print(len(valid_vv))
