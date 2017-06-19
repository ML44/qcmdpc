
# coding: utf-8

# # Reconstruction du spectre
# 
# Selon l'algorithme décrit dans le second papier de Guo et al.

# ## Algorithmes

# ### General tools

# In[338]:

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


# In[339]:

def get_spectrum(h,p):
    s = [0]*p
    for i in h:
        for j in h:
            s[(i-j) % p]=1
    return s


# ### Bron Kerbosch

# In[340]:

def get_neighbours(v,S,spectrum):
    n = []
    for i in S:
        if i!=v and spectrum[abs(i-v)] :
            n.append(i)
    return(set(n))


# In[341]:

def bron_kerbosch(S, spectrum, clique, candidates, excluded, list_cliques, maxweight):
    #Naive Bron–Kerbosch algorithm
    if not candidates and not excluded:
        if len(clique) >= maxweight:
            list_cliques.append(clique)
        return list_cliques
 
    for v in list(candidates):
        n = get_neighbours(v,S,spectrum)
        new_candidates = candidates.intersection(n)
        new_excluded = excluded.intersection(n)
        list_cliques = bron_kerbosch(S, spectrum, clique + [v], new_candidates, new_excluded, list_cliques, maxweight)
        candidates.remove(v)
        excluded.add(v)
    return list_cliques


# ### Tools for DSR

# In[342]:

def get_p1(spectrum):
    p1 = 1
    while (spectrum[p1]==0) :
        p1+=1
    return p1


# In[343]:

def construct_A(spectrum,p1):
    A = []
    for i in range(p1+1, len(spectrum)) :
        if spectrum[i] and spectrum[i-p1]:
            A.append(i)
    return A


# In[344]:

def construct_B(spectrum, A, p2):
    B = []
    for i in A:
        if i!=p2 and spectrum[i-p2]:
            B.append(i)
    return B


# In[345]:

def get_cliques(C, spectrum, weight):
    if len(C)==weight:
        C.sort()
        return [C]
    if len(C)>weight:
        cliques = bron_kerbosch(C, spectrum, [], set(C), set([]), [], weight)
        l = []
        for c in cliques:
            c.sort()
            if not (c in l):
                l.append(c)
    return l


# ### DSR algorithm

# In[346]:

def dsr(spectrum, weight):
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
        K = get_cliques(S, spectrum, weight)
        for k in K:
            if not(k in F):
                F.append(k)
    return F


# ## Tests

# In[349]:

def test(p,weight,N):
    for i in range(N):
        h = generate_h(p,weight)
        spectrum = get_spectrum(h,p)
        mydsr = dsr(spectrum, weight)
        if len(mydsr)>2:
            print("h = " + str(h))
            for h2 in mydsr:
                print("h2 = " + str(h2) + " : " + str(get_spectrum(h2,p)==spectrum))
            print("\n")        


# In[367]:

test(8192,13,1000)


# In[ ]:




# In[ ]:



