# cython: language_level=3

import matplotlib.pyplot as plt
from gintsieve_externs cimport gPrimes, gPrimesCount, gPrimesRegion



# cython casts vector to a list
# see https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html#standard-library


cpdef gprimes(long x):
    return gPrimes(x)

cpdef gprimes_count(long x):
    return gPrimesCount(x)

cpdef gprimes_region(long x, long y, long z):
    return gPrimesRegion(x, y, z)


def visualize(P, full_disk=False):
    """Plot Gaussian primes with Matplotlib."""

    dist = (P[-1][0] ** 2 + P[-1][1] ** 2) ** 0.5
    x = [p[0] for p in P]
    y = [p[1] for p in P]
    plt.subplots(figsize=(8, 8))
    plt.axhline(0, color='red')
    plt.axvline(0, color='red')
    if full_disk:
        # Filling out second quadrant
        x, y = x + [-t for t in y], y + x

        # Filling out third and fourth quadrants
        x += [-t for t in x]
        y += [-t for t in y]

        plt.plot(x, y, 'bo', markersize=100 / dist)

    else:
        plt.plot(x, y, 'bo', markersize=200 / dist)

    plt.show()
