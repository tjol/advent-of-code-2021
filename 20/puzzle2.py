#!/usr/bin/env python3

import sys
import numpy as np

table = (np.char.array(sys.stdin.readline(), unicode=False).view('u1', np.ndarray) == ord('#')).astype('u2')
far_field = 0
assert(sys.stdin.readline().strip() == '')
img = np.array([[c == '#' for c in line.strip()] for line in sys.stdin.readlines()], dtype='u2')

def expand(img):
    expanded = np.zeros((img.shape[0]+2, img.shape[1]+2), dtype='u2') + far_field
    expanded[1:-1,1:-1] = img
    return expanded

# initial expansion
img = expand(img)

for _ in range(50):
    img = expand(img)

    replacement_map = (
        (img[:-2,:-2] << 8)
        | (img[:-2,1:-1] << 7)
        | (img[:-2,2:] << 6)
        | (img[1:-1,:-2] << 5)
        | (img[1:-1,1:-1] << 4)
        | (img[1:-1,2:] << 3)
        | (img[2:,:-2] << 2)
        | (img[2:,1:-1] << 1)
        | (img[2:,2:]))

    far_field = table[0] if far_field == 0 else table[0b111111111]
    img[:,:] = far_field
    img[1:-1,1:-1] = table[replacement_map]

print(np.count_nonzero(img))
