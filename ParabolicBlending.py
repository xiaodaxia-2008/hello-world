
# coding: utf-8

# In[1]:


import matplotlib.pyplot as plt
import numpy as np

xs =np.array([0, 3, 2])
ts = np.array([0, 0.5, 1])

import sympy as sp
sp.init_printing()


# In[4]:


ta, tb, xa, xb, t= sp.symbols("t_a t_b x_a x_b t")
k1, b1, k2, b2, a0, a1, a2, a3 =sp.symbols("k1 b1 k2 b2 a0 a1 a2 a3")


# In[5]:


# x = sp.sympify("a0 + a1*t + a2*t**2")
# x1 = sp.sympify("k1*t + b1")
# x2 = sp.sympify("k2*t + b2")
x = sp.sympify("a0 + a1*t + a2*t**2")
x1 = sp.sympify("6*t")
x2 = sp.sympify("-2*t + 4")


# In[6]:


x, x1, x2


# In[7]:


# equations
eqs = [sp.Eq(x1.subs(t, ta), xa), sp.Eq(x2.subs(t, tb), xb),
      sp.Eq(x.subs(t, ta), xa), sp.Eq(x.subs(t, tb), xb), 
       sp.Eq(x.diff(t).subs(t, tb), -2), sp.Eq(x.diff(t).subs(t, ta), 6)]


# In[8]:


len(eqs), eqs


# In[9]:


ret = sp.nonlinsolve(eqs, [a0, a1, a2, xa, xb, tb])


# In[10]:


ret


# In[11]:


x = x.subs(((a0, ret.args[0][0]), (a1, ret.args[0][1]), (a2, ret.args[0][2]))).subs(ta, 0.4)


# In[12]:


inps = [[i, x1.subs(t, i)] for i in np.arange(0, 0.4, 0.01)]
inps.extend([[i, x.subs(t, i)] for i in np.arange(0.4, 0.6, 0.01)])
inps.extend([[i, x2.subs(t, i)] for i in np.arange(0.6, 1, 0.01)])


# In[13]:


inps = np.array(inps)


# In[18]:


plt.plot(inps[:, 0], inps[:, 1])
plt.plot(ts, xs, "--o")

