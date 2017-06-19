# coding: utf-8

import multiprocessing
import time


def instance(id):
    import random
    r = random.randint(0, 100)
    if r < 25:
        t = 5
    else:
        t = 10*r
    print("Essai "+str(id)+" ; tirage = "+str(r)+" ; timing = "+str(t))
    time.sleep(t)
    print("OK!")
    print("SECRET")


def aux(i):
    while True:
        p = multiprocessing.Process(target=instance, args=(i,))
        p.start()
        p.join(10)
        if p.is_alive():
            p.terminate()
            p.join()
        else:
            break

        
def main():
    print(aux(1))
