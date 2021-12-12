import numpy as np

def run(input):
    # brute force algo almost certainly not optimal
    fishes = np.fromstring(input, dtype='u1', sep=',')
    buckets, _ = np.histogram(fishes, bins=9, range=(-0.5, 8.5))
    for i in range(256):
        buckets = np.roll(buckets, -1)
        buckets[6] += buckets[8]
        if i == 79:
            eighty = buckets.sum()
    return eighty, buckets.sum()
