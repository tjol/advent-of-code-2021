#!/usr/bin/env python3

import sys
import numpy as np

input_map = np.array([np.char.array(line.strip().encode('us-ascii'), unicode=False).view('u1', np.ndarray) - ord('0') for line in sys.stdin if line])
h, w = input_map.shape
heightmap = np.zeros((h+2, w+2), dtype='u1')
heightmap += 9
main_map = heightmap[1:-1,1:-1]
main_map[:,:] = input_map
is_low = main_map < heightmap[:-2,1:-1]
is_low &= main_map < heightmap[2:,1:-1]
is_low &= main_map < heightmap[1:-1,:-2]
is_low &= main_map < heightmap[1:-1,2:]
print(np.sum(main_map[is_low] + 1))
