import numpy as np

def rating(space, bit_idx, o2_mode):
    mask = (1 << bit_idx) | (space[0] & (0xffff << bit_idx+1))
    zeros = np.searchsorted(space, mask)
    ones = len(space) - zeros
    if (o2_mode and ones >= zeros) or (not o2_mode and ones < zeros) or (zeros == 0):
        # keep the ones
        if ones == 1:
            return space[zeros]
        else:
            return rating(space[zeros:], bit_idx-1, o2_mode)
    else:
        # keep the zeros 
        if zeros == 1:
            return space[0]
        else:
            return rating(space[:zeros], bit_idx-1, o2_mode)

def run(input):
    # diag = np.array([int(l, 2) for l in input.splitlines() if l])
    n_digits = input.find('\n')
    n_chars_per_line = n_digits+1
    input_arr = np.char.array(input + '!', unicode=False).view('u1', np.ndarray)
    items = input_arr.size // n_chars_per_line # floor
    input_arr = input_arr[:items*n_chars_per_line].reshape(items, n_chars_per_line)[:,:-1]
    input_arr -= 48
    bit_counts = np.sum(input_arr, axis=0)
    bit_shifts = np.arange(n_digits - 1, -1, -1)
    gamma_bits = bit_counts > items//2
    gamma = np.sum(gamma_bits.astype('u4') << bit_shifts)
    epsilon = np.sum((~gamma_bits).astype('u4') << bit_shifts)
    # now convert to numbers
    diag_nums = np.sum(input_arr.astype('u2') << bit_shifts, axis=1)
    diag_nums.sort()
    o2_rating = rating(diag_nums, n_digits-1, True)
    co2_rating = rating(diag_nums, n_digits-1, False)

    # raise NotImplementedError
    return gamma*epsilon, o2_rating * co2_rating
