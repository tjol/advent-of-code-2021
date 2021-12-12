import numpy as np
import scipy.ndimage

def run(input):
    input += '~'
    w = input.find('\n')
    w_ = w+1
    h = len(input) // (w+1)
    heightmap = np.char.array(input, unicode=False).view('u1', np.ndarray)[:h*w_].reshape(h, w_)[:,:w] - 48

    # part 2
    labels, n_features = scipy.ndimage.label(heightmap != 9)
    sizes = [np.count_nonzero(labels == i) for i in range(1, n_features+1)]
    sizes.sort()
    top3_prod = sizes[-1] * sizes[-2] * sizes[-3]

    return n_features, top3_prod
