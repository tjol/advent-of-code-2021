#!/usr/bin/env python3

import imageio
import numpy as np
import sys

fn = sys.argv[1]

img = np.loadtxt(fn, dtype='u1', delimiter=',')
maxval = np.max(img)
factor = 0xff // maxval
img *= factor

if fn.endswith('csv'):
    out_fn = fn.replace('csv', 'png')
else:
    out_fn = fn + '.png'

imageio.imwrite(out_fn, img)
