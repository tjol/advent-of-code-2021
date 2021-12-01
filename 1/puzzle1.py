#!/usr/bin/env python3

import sys

depths = list(map(int, sys.stdin))

increased = [a > b for (a, b) in zip(depths[1:], depths[:-1])]

print(sum(increased))
