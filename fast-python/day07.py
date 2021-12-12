import numpy as np
import scipy.optimize

def run(input):
    crabs = np.fromstring(input, dtype='i4', sep=',')

    def costfn1(x):
        return np.abs(crabs - x).sum()
    
    def costfn2(x):
        n = np.abs(crabs - x)
        return ((n * (n + 1)) / 2).sum()

    cost1 = costfn1(int(np.round(scipy.optimize.minimize_scalar(costfn1).x)))
    cost2 = int(costfn2(np.round(scipy.optimize.minimize_scalar(costfn2).x)))
    return cost1, cost2

