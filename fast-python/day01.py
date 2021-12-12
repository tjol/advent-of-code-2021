import numpy as np
import scipy.signal

def run(input):
    nums = np.fromstring(input, dtype='i4', sep='\n')
    part1 = np.count_nonzero(nums[1:] > nums[:-1])
    wnd = np.ones(3, dtype='i4')
    window_sums = scipy.signal.convolve(nums, wnd, mode='valid')
    part2 = np.count_nonzero(window_sums[1:] > window_sums[:-1])
    return part1, part2
