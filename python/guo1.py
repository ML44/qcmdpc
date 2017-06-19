# coding: utf-8


import random


def generate_h(p, w):
    """
    Generate a random vector of size p and weight w
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
    Get the spectrum of a vector of size p
    """
    s = [0]*p
    for i in h:
        for j in h:
            s[(i-j) % p] = 1
    return s


def dsr(spectrum, key, atester, w):
    print("=================START================")
    print(key)
    print(atester)
    if len(key) == w:
        print(key)
        return 1
    for k in atester:
        print("Testing "+str(k))
        b = True
        for i in key[1:]:
            print("\tIsDist "+str(i))
            print("\t\tDist = "+str(abs(k-i)))
            if spectrum[abs(k-i)] == 0:
                print("\t\tNO")
                b = False
                break
            else:
                print("\t\tYES")
        if b:
            key.append(k)
            atester.remove(k)
            i = (dsr(spectrum, key, atester, w))
            key.remove(k)
    print("==================END=================")
    return 0


def dsr_tot(spectrum, w):
    atester = []
    for i in range(1, len(spectrum)):
        if spectrum[i] == 1:
            atester.append(i)
    p1 = atester[0]
    key = [0, p1]
    atester.remove(p1)
    return (dsr(spectrum, key, atester, w))


# -------------------------------------
# TESTS
# -------------------------------------


if __name__ == "__main__":
    import sys
    p = int(sys.argv[1])
    w = int(sys.argv[2])
    h0 = generate_h(p, w)
    print(h0)
    s0 = get_spectrum(h0, p)
    print(s0)
    h1 = dsr_tot(s0, w)
    print(h1)
