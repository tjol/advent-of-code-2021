import numpy as np
import scipy.ndimage

def run(input):
    input += '~'
    w = input.find('\n')
    w_ = w+1
    h = len(input) // (w+1)
    heightmap = np.zeros((h+2, w+2), dtype='u1') + 9
    main_map = heightmap[1:-1,1:-1]
    main_map[:,:] = np.char.array(input, unicode=False).view('u1', np.ndarray)[:h*w_].reshape(h, w_)[:,:w] - 48

    is_low = main_map < heightmap[:-2,1:-1]
    is_low &= main_map < heightmap[2:,1:-1]
    is_low &= main_map < heightmap[1:-1,:-2]
    is_low &= main_map < heightmap[1:-1,2:]
    danger_score = np.sum(main_map[is_low] + 1)

    # part 2
    labels, n_features = scipy.ndimage.label(main_map != 9)
    sizes = [np.count_nonzero(labels == i) for i in range(1, n_features+1)]
    sizes.sort()
    top3_prod = sizes[-1] * sizes[-2] * sizes[-3]

    return danger_score, top3_prod
