
# coding: utf-8

# # Reconstruction du spectre
# 
# Selon l'algorithme décrit dans le second papier de Guo et al.

# ## Algorithmes

# ### General tools

# In[1]:

import random
def generate_h(p,w):
    h = [0]
    while len(h)<w:
        k = random.randint(0,p-1)
        while(k in h):
            k = random.randint(0,p-1)
        h.append(k)
    h.sort()
    return h


# In[2]:

def get_spectrum(h,p):
    s = [0]*p
    for i in h:
        for j in h:
            s[(i-j) % p]=1
    return s


# ### Bron Kerbosch

# In[3]:

def get_neighbours(v,S,spectrum):
    n = []
    for i in S:
        if i!=v and spectrum[abs(i-v)] :
            n.append(i)
    return(set(n))


# In[4]:

def bron_kerbosch(S, spectrum, clique, candidates, excluded, maxweight):
    if len(clique)>=maxweight:
        return clique
 
    #Naive Bron–Kerbosch algorithm
    if not candidates and not excluded:
        return []
 
    for v in list(candidates):
        n = get_neighbours(v,S,spectrum)
        new_candidates = candidates.intersection(n)
        new_excluded = excluded.intersection(n)
        result = bron_kerbosch(S, spectrum, clique + [v], new_candidates, new_excluded, maxweight)
        if len(result)>0:
            return result
        candidates.remove(v)
        excluded.add(v)
    return []


# ### Tools for DSR

# In[5]:

def get_p1(spectrum):
    p1 = 1
    while (spectrum[p1]==0) :
        p1+=1
    return p1


# In[6]:

def construct_A(spectrum,p1):
    A = []
    for i in range(p1+1, len(spectrum)) :
        if spectrum[i] and spectrum[i-p1]:
            A.append(i)
    return A


# In[7]:

def construct_B(spectrum, A, p2):
    B = []
    for i in A:
        if i!=p2 and spectrum[i-p2]:
            B.append(i)
    return B


# In[8]:

def get_cliques(C, spectrum, weight):
    if len(C)>=weight:
        cliques = bron_kerbosch(C, spectrum, [], set(C), set([]), [], weight)
        l = []
        for c in cliques:
            c.sort()
            if not (c in l):
                l.append(c)
    return l


# ### DSR algorithm

# In[9]:

def dsr(spectrum, weight, cliquesize):
    # 0. Identify p1
    p1 = get_p1(spectrum)
    # 1. Identify 3-cliques
    A = construct_A(spectrum, p1)
    # 2. Combine 3-cliques
    E = []
    for p2 in A:
        B = construct_B(spectrum, A, p2)
        C = [0,p1,p2]
        C.extend(B)
        C.sort() # Useless and time consuming ?
        #print("p2 = "+str(p2)+" --> C ="+str(C))
        b = True
        while len(C)>=weight and b :
            for i in C:
                c = 0
                for j in C:
                    if spectrum[abs(j-i)]:
                        c+=1
                if c<weight:
                    C.remove(i)
                    break
                if i == C[-1]:
                    b = False
        if not(b):
            E.append(C)
    # 3. F = cliques of E if elements of E are too large
    F = []
    for S in E:
        K = get_cliques(S, spectrum, cliquesize)
        for k in K:
            if not(k in F):
                F.append(k)
    return F


# ## Tests

# In[53]:

def translation_index(h,p,p1):
    for i in range(len(h)-1):
        if h[i+1]-h[i] == p1:
            return i
    if h[0]+p-h[-1]==p1:
        return len(h)-1
    return -1


# In[66]:

def translation(h,p,p1):
    k = translation_index(h,p,p1)
    ht = []
    val = h[k]
    for i in range(len(h)):
        nhk = (h[k]-val)%p
        ht.append(nhk)
        k = (k+1) % len(h)
    return ht


# In[79]:

def test_kb(p,weight,N,cliquesize):
    for i in range(N):
        h = generate_h(p,weight)
        print(h)
        spectrum = get_spectrum(h,p)
#        print(spectrum)
        candidates = []
        for i in range(p):
            if (spectrum[i]):
                candidates.append(i)
        p1 = get_p1(spectrum)
        h_translation = translation(h,p,p1)
        print(h_translation)
        A = construct_A(spectrum, p1)
        l = bron_kerbosch(range(p), spectrum, [0,p1], set(A), set([]), cliquesize)
        l.sort()
        print(l)
        spectrum_l = get_spectrum(l,p)
#        print(spectrum_l)
        reconstruct = [8]*p
        for i in range(len(l)):
            reconstruct[l[i]]=1
            for j in range(p):
                if spectrum[j]==0:
                    reconstruct[(l[i]+j)%p]=0
                    reconstruct[(l[i]-j)%p]=0
        
        spectrum_verif = get_spectrum(l,p)
        b = True
        for i in range(p):
            if spectrum_verif[i] and not(spectrum[i]):
                b = False
                break
        print(b)
        
#        print(reconstruct)
        print("\n=====================\n")


# In[82]:

test_kb(4801,45,1,20)


# In[25]:

192-62


# In[70]:

5000-1024


# In[1]:

45*22


# In[4]:

import numpy as np
np.log(4801)


# In[5]:

2+2


# In[10]:

p = 45*44/(4801*2)


# In[19]:

np.log(4801)/np.log(1/p)


# In[16]:

44*45/(2*4801)


# In[17]:

p


# In[18]:

44*45/2


# In[20]:

import sympy


# In[27]:

sympy.binomial(4801,9)*(p**(5*9))


# In[31]:

4801*(p**6)


# In[33]:

np.log(2400*100000)/np.log(2)


# In[ ]:



