# coding: utf-8


import random


# --------------------------------------------
# Basic vector generation
# --------------------------------------------

def generate_h(p, w):
    """
    Generate a random vector of size p and weight w.
    """
    h = [0]
    while len(h) < w:
        k = random.randint(0, p-1)
        while(k in h):
            k = random.randint(0, p-1)
        h.append(k)
    h.sort()
    return h


def get_spectrum(h, p):
    """
    Get the spectrum of a vector of size p.
    """
    s = [0]*p
    for i in h:
        for j in h:
            s[(i-j) % p] = 1
    return s


# --------------------------------------------
# Key manipulation
# --------------------------------------------
"""
The key is represented by:
* an array containing:
 - 0 (if the bit is not present),
 - 1 (if the bit is there for sure),
 - 2 (if we don't know)
* a counter of 0s, 1s and 2s.
"""


def dist(i, j, spectrum):
    """
    Returns whether distance between i and j is in the spectrum.
    """
    return spectrum[abs(j-i)]


def get_bit(k, i):
    """
    Returns the ith bit of the key k.
    """
    return k[0][i]


def set_bit(k, i, v):
    """
    Sets the ith bit of the key k to value v.
    """
    b = get_bit(k, i)
    assert(b == 2)
    k[0][i] = v
    k[1][b] -= 1
    k[1][v] += 1
    return k


def init_key(spectrum):
    """
    Returns a key full of "2" (ie. unknown values)
    and performs the first round starting with bit 1.
    """
    p = len(spectrum)
    k = [[2]*p, [0, 0, p]]
    k = set_bit(k, 0, 1)
    for i in range(p):
        if dist(0, i, spectrum) == 0:
            k = set_bit(k, i, 0)
    return k


def add(k, s, i):
    """
    Tries to change the ith bit of k to 1
    while matching the spectrum constrains.
    Returns wether it worked or not.
    """
    assert(get_bit(k, i) == 2)
    b = True
    j = 0
    while (b and j < len(s)):
        b2 = (dist(i, j, s) == 0) and (get_bit(k, j) == 1)
        if b2:
            print(j)
        b &= not(b2)
        j += 1
    if b:
        k = set_bit(k, i, 1)
        for j in range(len(s)):
            if ((dist(i, j, s) == 0) and (get_bit(k, j) == 2)):
                k = set_bit(k, j, 0)
    return b


def aux(k, w, s, i, l):
    """
    Auxiliary function to reconstruct the key.
    """
    if len(l) != 0:
        return l
    if (k[1][1] == w):
        # print("KEY found:")
        # print_key(k)
        l.append(extract(k))
    elif (k[1][1] + k[1][2] >= w):
        for j in range(i, len(s)):
            if get_bit(k, j) == 2:
                k2 = [k[0][:], k[1][:]]
                b = add(k2, s, j)
                if b:
                    l = aux(k2, w, s, j+1, l)
    return l


def reconstruct(s, w):
    """
    Reconstruction function for a spectrun s and a weight w.
    """
    k = init_key(s)
    return aux(k, w, s, 1, [])[0]


def extract(k):
    """
    Returns a sparse version of the key.
    """
    h = []
    for i in range(len(k)):
        if get_bit(k, i) == 1:
            h.append(i)
    return h


def verify(h1, h2, p):
    """
    Checks if h1 and h2 have same spectrum.
    """
    s1 = get_spectrum(h1, p)
    s2 = get_spectrum(h2, p)
    return(s1 == s2)


# def spectrum_noise(s, a):
#     for i in range(len(s)):
#         r = random.random()
#         if r > a:
#             s[i] = 2
#     return s


# --------------------------------------------
# Test section
# --------------------------------------------


def test(p, w):
    h1 = generate_h(p, w)
    print(h1)
    s1 = get_spectrum(h1, p)
    print(s1)
    k2 = reconstruct(s1, w)
    print(k2)
    h2 = extract(k2)
    print(h2)
    return(verify(h1, h2, p))


# def testN(p, w, N):
#     s = 0
#     for i in range(N):
#         s += test(p, w)
#     return s/N


# p=300
# w=11
# a=0.8
# h = generate_h(p,w)
# s = get_spectrum(h,p)
# spectrum_noise(s,a)
# import time
# t0 = time.time()
# k = reconstruct(s,w)
# t1 = time.time()
# print(t1-t0)
# verify(k,h,p)
# h
