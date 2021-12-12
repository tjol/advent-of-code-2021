import numpy as np
import itertools
from scipy.signal import convolve2d

flash_kernel = np.array([[1, 1, 1],
                         [1, 0, 1],
                         [1, 1, 1]], dtype='u1')

def run(input):
    input += '~'
    w = input.find('\n')
    w_ = w+1
    h = len(input) // (w+1)
    energy_map = np.char.array(input, unicode=False).view('u1', np.ndarray)[:h*w_].reshape(h, w_)[:,:w] - 48

    all_flashes = np.zeros_like(energy_map, dtype=bool)

    flash_count = 0
    flashes_after_100 = 0

    for i in itertools.count(1):
        energy_map += 1
        all_flashes[:,:] = False
        flashes_this_step = 0

        while True:
            just_flashed = energy_map > 9
            just_flashed_count = np.count_nonzero(just_flashed)
            if just_flashed_count == 0:
                break
            flashes_this_step += just_flashed_count
            all_flashes |= just_flashed
            # increment the neighbours
            neighbours = convolve2d(just_flashed.astype('u1'), flash_kernel, mode='same')
            energy_map += neighbours
            energy_map[all_flashes] = 0

        flash_count += flashes_this_step

        if i == 100:
            flashes_after_100 = flash_count

        if flashes_this_step == 100:
            return flashes_after_100, i
