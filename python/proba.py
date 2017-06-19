# coding: utf-8

import numpy as np
import math


def vect(n, t):
    e = [0]*n
    i = 0
    while (i < t):
        b = np.random.randint(n)
        if e[b] == 0:
            e[b] = 1
            i += 1
    return e


def is_dist_in(v, d):
    for i in range(len(v)):
        if v[i] and v[(i+d) % len(v)]:
            return 1
    return 0


def proba_dist(n, t, d, N):
    s = 0
    for i in range(N):
        v = vect(n, t)
        s+=is_dist_in(v, d)
    return s/N


proba_dist(4801,45,42,10000)


# In[13]:

def proba_th0(n,t):
    s1 = 1
    s2 = 1
    for k in range(t):
        s1*=(n-3*k)
        s2*=(n-k)
    return s1/s2


# In[14]:

proba_th0(4801,45)


# In[15]:

def proba_th1(n,t):
    s1 = 1
    s2 = 1
    for k in range(t-2):
        s1*=(n-4-3*k)
        s2*=(n-2-k)
    return 0.5*s1/s2


# In[16]:

proba_th1(4801,45)


# In[17]:

def proba_th_plus(n,t):
    return 1 - proba_th0(n,t) - proba_th1(n,t)


# In[18]:

proba_th_plus(4801,45)


# In[19]:

def log_fac(k):
    s = 0
    for i in range(1,k+1):
        s+=math.log(i)
    return s


# In[20]:

log_fac(4)


# In[21]:

def ccp_j(u,b):
    gamma = 0.577215664
    return u * (np.log(u) + (b-1)*u*np.log(u)+gamma-log_fac(b-1))


# In[22]:

ccp_j(1401,10) / (45 * 22)


# In[23]:

N = 100000


# In[24]:

alpha = proba_th1(4801,45)


# In[25]:

1/math.sqrt(N*alpha)


# In[ ]:




# In[26]:

def scal_prod_rot(h,e,k):
    l = len(h)
    s = 0
    for i in range(l):
        s = (s + e[i]*h[(i+k)%l])%2
    return s


# In[27]:

def sigma0_one(p,wh,we,d):
    h = vect(p,wh)
    e = vect(p,we)
    while(is_dist_in(e,d)):
        e = vect(p,we)
    ws = 0
    for i in range(p):
        ws+=scal_prod_rot(h,e,i)
    return ws


# In[28]:

sigma0_one(4801,45,42,3)


# In[ ]:




# In[29]:

import sympy


# In[30]:

def proba_s1(n,w,t):
    s = 0.0
    for l in range(t+1):
        if l%2==1:
            s+=sympy.binomial(w,l) * sympy.binomial(n-w,t-l) / sympy.binomial(n,t)
    return float(s)



# In[31]:

proba_s1(4801,45,42)


# In[32]:

_*4801


# In[33]:

0.06/4802


# In[34]:

import scipy.stats as stat


# In[35]:

q = stat.norm.ppf(0.9999,loc=0,scale=1)


# In[36]:

sigma = 11.8


# In[37]:

epsillon = 0.3


# In[38]:

(epsillon/(2*sigma*q))**-2


# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[39]:

def sigma_average(n,w,t):
    s = 0.0
    for l in range(t+1):
        if l%2==1:
            s+=sympy.binomial(w,l) * sympy.binomial(n-w,t-l) / sympy.binomial(n,t)
    s = s*n
    return float(s)


# In[40]:

sigma_average(4801, 45, 42)


# In[41]:

sympy.binomial(4801, 45)


# In[42]:

0.271745 * 4801


# In[43]:

1324/4801


# In[44]:

def sig(n,w,t):
    s = 0
    for l in range(t+1):
        if (l%2==1) : 
            c = sympy.binomial(w,l)
            for i in range(1,t-l+1): 
                c *= (n-w+1-i) / (n+1-i)
            for  i in range(t-l+1,t+1):
                c *= (i) / (n+1-i)
            s = s+c;
    s*=n
    return s


# In[45]:

sig(4801,45,42)


# In[46]:

def sigma_average0(n,w,t):
    s = 0
    for l in range(t+1):
        if l%2==1:
            s+=sympy.binomial(w,l) * sympy.binomial(n-w,t-l) / sympy.binomial(n,t)
    s = s*(n-1)
    c = 0.0
    for l in range(t-1):
        if l%2==1:
            c+=sympy.binomial(w-2,l) * sympy.binomial(n-w,t-2-l) / sympy.binomial(n-2,t-2)
    s+=c
    return float(s)


# In[47]:

sigma_average0(4801, 45, 42)


# In[48]:

0.02*45


# In[49]:

def f(n,w,t,b):
    s = 0
    for l in range(t+1):
        if l%2==b:
            s+=sympy.binomial(w,l) * sympy.binomial(n-w,t-l) / sympy.binomial(n,t)
    return float(s)


# In[50]:

4801*f(4801, 45, 42, 1)


# In[51]:

45*(f(4801-2, 45-1, 42-2, 0)+1) + (4801-45)*f(4801-2, 45-1, 42-2, 1)


# In[ ]:




# In[52]:

def sigma(l,n,w,t):
    return (n-2*w+l)*f(n-2,w,t-2,1) + 2*(w-l)*f(n-2,w-1,t-2,0) + l*f(n-2,w-2,t-2,1)


# In[53]:

sigma(0,4801,45,42)


# In[54]:

sigma(1,4801,45,42)


# In[ ]:





# In[ ]:




# In[ ]:




# In[1]:

alpha = 0.34


# In[9]:

eps = 0.0003


# In[10]:

1/(eps**2 * alpha * 4801)


# In[8]:

0.4/1324


# In[11]:

109000/6806


# In[ ]:



