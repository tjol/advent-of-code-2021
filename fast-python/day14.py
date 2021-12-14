import numpy as np
from scipy.sparse import coo_matrix

N = 26 * 26

def run(input):
    template, mapping_info = input.split('\n\n', 1)

    # turn the mapping into a matrix
    mapping_array = np.char.array(mapping_info, unicode=False).view('i1', np.ndarray) - 65 # ord('A')
    from1 = mapping_array[::8].astype('i2')
    from2 = mapping_array[1::8].astype('i2')
    from_merged = from1 * 26 + from2
    to = mapping_array[6::8].astype('i2')
    to1_merged = from1 * 26 + to
    to2_merged = to * 26 + from2

    n_rules = len(from1)
    matrix = coo_matrix((np.ones(n_rules, dtype='i8'), (to1_merged, from_merged)), shape=(N, N))
    matrix += coo_matrix((np.ones(n_rules, dtype='i8'), (to2_merged, from_merged)), shape=(N, N))
    matrix = matrix.tocsr()

    # turn the template into a suitable vector
    vec = np.zeros(N, dtype='i8')
    tpl_arr = (np.char.array(template, unicode=False).view('i1', np.ndarray) - 65).astype('i2')
    indices = tpl_arr[:-1] * 26 + tpl_arr[1:]
    for i in indices:
        vec[i] += 1

    for _ in range(10):
        vec = matrix @ vec

    part1result = calc_result(vec, ord(template[0]) - 65, ord(template[-1]) - 65)

    for _ in range(30):
        vec = matrix @ vec

    part2result = calc_result(vec, ord(template[0]) - 65, ord(template[-1]) - 65)

    return part1result, part2result

def calc_result(vec, first, last):
    vec_2d = vec.reshape(26, 26)
    counts = (vec_2d + vec_2d.T).sum(axis=0)
    counts[first] += 1
    counts[last] += 1
    most = counts.max()
    least = counts[counts != 0].min()
    return (most - least) // 2
