#!/usr/bin/env python3

import sys
import re

m = re.match(r'^target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+)', sys.stdin.readline())
y_min = min(int(m.group(3)), int(m.group(4)))

print((y_min * (y_min + 1)) // 2)
