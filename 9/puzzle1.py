#!/usr/bin/env python3

import sys
import numpy as np

def neighbours(heightmap, i, j):
    result = []
    if i > 0:
        result.append(heightmap[i-1, j])
    if i < heightmap.shape[0] - 1:
        result.append(heightmap[i+1, j])
    if j > 0:
        result.append(heightmap[i, j-1])
    if j < heightmap.shape[1] - 1:
        result.append(heightmap[i, j+1])
    return np.array(result)

def lowpoint_risklevels(heightmap):
    for i in range(heightmap.shape[0]):
        for j in range(heightmap.shape[1]):
            h = heightmap[i,j]
            if np.all(h < neighbours(heightmap, i, j)):
                yield 1 + h

heightmap = np.array([[ord(c) - ord('0') for c in line.strip()] for line in sys.stdin])
print(sum(lowpoint_risklevels(heightmap)))
