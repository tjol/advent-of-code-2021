#!/usr/bin/env python3

import sys
import numpy as np
import scipy.ndimage

heightmap = np.array([np.char.array(line.strip().encode('us-ascii'), unicode=False).view('u1', np.ndarray) - ord('0') for line in sys.stdin if line])
labels, n_features = scipy.ndimage.label(heightmap != 9)
sizes = [np.count_nonzero(labels == i) for i in range(1, n_features+1)]
sizes.sort()
print(sizes[-1] * sizes[-2] * sizes[-3])


'''
import sys
import numpy as np
import functools
import operator

def neighbour_indices(heightmap, i, j):
    if i > 0:
        yield (i-1, j)
    if i < heightmap.shape[0] - 1:
        yield (i+1, j)
    if j > 0:
        yield (i, j-1)
    if j < heightmap.shape[1] - 1:
        yield (i, j+1)

def neighbours(heightmap, i, j):
    return np.array([heightmap[idx] for idx in neighbour_indices(heightmap, i, j)])


def flood(heightmap):
    basin_map = np.zeros_like(heightmap)
    height, width = heightmap.shape

    next_basin = 1

    # raise the water level
    for level in range(9):
        for i in range(height):
            for j in range(width):
                h = heightmap[i, j]
                if h == level:
                    # We're up!
                    basins = set(basin
                        for (i2, j2) in neighbour_indices(heightmap, i, j)
                        if (basin := basin_map[i2, j2]) != 0)
                    if not basins:
                        # New low point
                        basin_map[i, j] = next_basin
                        next_basin += 1
                    elif len(basins) == 1:
                        basin_map[i, j] = list(basins)[0]
                    else:
                        raise ValueError("Ambiguous non-9 ridge")
    return basin_map

def basin_sizes(basin_map):
    basin_id = 1
    while True:
        size = np.count_nonzero(basin_map == basin_id)
        if size != 0:
            yield size
        else:
            return
        basin_id += 1


heightmap = np.array([[ord(c) - ord('0') for c in line.strip()] for line in sys.stdin])

basins = flood(heightmap)
# print(basins)
sizes_list = sorted(basin_sizes(basins))
print(functools.reduce(operator.mul, sizes_list[-3:], 1))

# from matplotlib import pyplot as plt
# basins_f = basins.astype('float')
# basins_f[basins_f == 0] = np.nan
# plt.figure(figsize=(8, 8))
# plt.axes((0, 0, 1, 1), frame_on=False)
# plt.imshow(basins_f, cmap='gist_rainbow')
# plt.savefig('basins.png')
'''