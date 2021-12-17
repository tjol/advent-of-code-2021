#!/usr/bin/env python3

import sys
import re

m = re.match(r'^target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+)', sys.stdin.readline())
x_min = int(m.group(1))
x_max = int(m.group(2))
y_min = int(m.group(3))
y_max = int(m.group(4))


def y_path(v_y, lim=None):
    y = 0
    while lim is None or y > lim:
        y += v_y
        v_y -= 1
        yield y

def x_path(v_x, lim=None):
    x = 0
    while (lim is None or x < lim):
        x += v_x
        if v_x > 0:
            v_x -= 1
        yield x


neg_vys = [v_y for v_y in range(y_min, 0) if any(y_min <= y <= y_max for y in y_path(v_y, y_min))]
vys = neg_vys + [- 1 - v_y for v_y in neg_vys[::-1]]

count = 0

for v_y in vys:
    for v_x in range(x_max+1):
        if any(x_min <= x <= x_max and y_min <= y <= y_max 
                for x, y in zip(x_path(v_x, x_max), y_path(v_y, y_min))):
            count += 1

print(count)
