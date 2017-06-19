
# coding: utf-8

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


# In[3]:

def dist(i,j,spectrum):
    return spectrum[abs(j-i)]


# In[4]:

def get_bit(k,i):
    return k[0][i]


# In[370]:

def set_bit(k,i,v):
    b = get_bit(k,i)
    assert(b==2)
    k[0][i] = v
    k[1][b] -= 1
    k[1][v] += 1
    return k


# In[371]:

def init_key(spectrum):
    p = len(spectrum)
    k = [[2]*p,[0,0,p]]
    k = set_bit(k,0,1)
    for i in range(p):
        if dist(0,i,spectrum)==0:
            k = set_bit(k,i,0)
    return k


# In[372]:

def add(k,s,i):
    assert(get_bit(k,i)==2)
    b = True
    j = 0
    while (b and j<len(s)):
        b2 = (dist(i,j,s)==0) and (get_bit(k,j)==1)
        if b2:
            print(j)
        b &= not(b2)
        j += 1
    if b:
        k = set_bit(k,i,1)
        for j in range(len(s)):
            if ((dist(i,j,s)==0) and (get_bit(k,j)==2)):
                k = set_bit(k,j,0)
    return b


# In[373]:

def extract(k):
    h=[]
    for i in range(len(k[0])):
        if get_bit(k,i)==1:
            h.append(i)
    return h


# In[374]:

def test(p,w):
    h = generate_h(p,w)
    print("h = "+str(h))
    s = get_spectrum(h,p)
    print("s = "+str(s))
    k = init_key(s)


# In[375]:

def test(p,w):
    h = generate_h(p,w)
    #print("h = "+str(h))
    s = get_spectrum(h,p)
    k = init_key(s)
    #print("Round 0: reste "+str(k[1][2]+k[1][1]-w))
    for i in range(1,len(h)):
        add(k,s,h[i])
        u = k[1][2]+k[1][1]-w
        #print("Round "+str(i)+": reste "+str(u))
        if u==0:
            break
    return(i)


# In[376]:

def testN(p,w,N):
    s=0
    for i in range(N):
        s+=test(p,w)
    return s/N


# In[377]:

# testN(4801,45,1000)


# In[ ]:




# In[379]:

def reconstruct(s,w):
    k = init_key(s)
    return aux(k,w,s,1,[],1,1)


# In[474]:

def aux(k,w,s,i,l,reclev,maxrec):
    maxrec = max(reclev,maxrec)
    if len(l)!=0:
        return (l,maxrec)
    if (k[1][1]==w):
        #print("KEY found:")
        #print_key(k)
        l.append(extract(k))
    elif (k[1][1]+k[1][2] >= w):
        for j in range(i,len(s)):
            if get_bit(k,j)==2:
                k2 = [k[0][:],k[1][:]]
                b = add(k2,s,j)
                if (b and k2[1][1]+k2[1][2]==w):
                    l,maxrec = aux(k2,w,s,j+1,l,reclev,maxrec)
                elif b:
                    l,maxrec = aux(k2,w,s,j+1,l,reclev+1,maxrec)
    return (l,maxrec)

def verify(k,h,p):
    s = get_spectrum(h,p)
    for i in range(len(k)):
        s2 = get_spectrum(k[i],p)
        print(str(s==s2)+" : "+str(k[i]))


import time

p=4801
w=45
N=100

for i in range(N):
    h = generate_h(p,w)
    s = get_spectrum(h,p)
    t0 = time.time()
    k,mr = reconstruct(s,w)
    t1 = time.time()
    print(str(i)+", \t"+str(mr)+",\t\t"+str(t1-t0))
