#!/usr/bin/env python3

import sys

depths = list(map(int, sys.stdin))

windows = [depths[i:i+3] for i in range(len(depths)-2)]
sums = [sum(wnd) for wnd in windows]
increased = [a > b for (a, b) in zip(sums[1:], sums[:-1])]

print(sum(increased))
