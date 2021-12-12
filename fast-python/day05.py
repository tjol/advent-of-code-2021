import numpy as np
def run(input):
    w = 1000
    img1 = np.zeros([w, w], dtype='u1')
    img2 = np.zeros([w, w], dtype='u1')

    for line in input.splitlines():
        p1, p2 = (np.fromstring(s, dtype='i4', sep=',') for s in line.split(' -> '))
        delta = p2 - p1
        dist = np.abs(delta).max()
        unit_d = delta // dist
        indices = np.outer(np.arange(dist+1), unit_d) + p1
        flat_indices = w * indices[:,0] + indices[:,1] 
        img2.flat[flat_indices] += 1
        if 0 in unit_d:
            img1.flat[flat_indices] += 1

    return np.count_nonzero(img1 > 1), np.count_nonzero(img2 > 1)

