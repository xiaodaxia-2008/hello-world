
# coding: utf-8

# In[1]:


get_ipython().run_line_magic('matplotlib', 'notebook')


# In[130]:


from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import

import matplotlib.pyplot as plt
import numpy as np

def arrow(ax, T, length=1):
    ax.quiver(T[0, 3], T[1, 3], T[2, 3],T[0, 0], T[0, 1], T[0, 2], length=length, normalize=True, color=(1, 0, 0, 0.5))
    ax.quiver(T[0, 3], T[1, 3], T[2, 3], T[1, 0],T[1, 1], T[1, 2], length=length, normalize=True, color=(0, 1, 0, 0.5))
    ax.quiver(T[0, 3], T[1, 3], T[2, 3], T[2, 0], T[2, 1], T[2, 2], length=length, normalize=True, color=(0, 0, 1, 0.5))

def rotate(axis, deg):
    AXIS = ('X', 'Y', 'Z')
    axis = str(axis).upper()

    if axis not in AXIS:

        print(f"{axis} is unknown axis, should be one of {AXIS}")

        return

    rot_x = axis == 'X'

    rot_y = axis == 'Y'

    rot_z = axis == 'Z'

    rot_mat = np.array([[(np.cos(deg), 1)[rot_x], (0, -np.sin(deg))[rot_z], (0, np.sin(deg))[rot_y], 0],

                        [(0, np.sin(deg))[rot_z], (np.cos(deg), 1)[rot_y], (0, -np.sin(deg))[rot_x], 0],

                        [(0, -np.sin(deg))[rot_y], (0, np.sin(deg))[rot_x], (np.cos(deg), 1)[rot_z], 0],

                        [0, 0, 0, 1]], dtype=np.float32)

    rot_mat = np.where(np.abs(rot_mat) < 1e-10, 0, rot_mat)  # get a small value when np.cos(np.pi/2)

    return rot_mat





def trans(axis, dis):
    AXIS = ('X', 'Y', 'Z')
    axis = str(axis).upper()

    if axis not in AXIS:

        print(f"{axis} is unknown axis, should be one of {AXIS}")

        return

    trans_mat = np.eye(4)

    trans_mat[AXIS.index(axis), 3] = dis

    return trans_mat


# In[131]:


T = trans("x", 3).dot(trans("y", 3)).dot(trans("z", 4)).dot(rotate("x", np.pi/3))


# In[132]:


T


# In[133]:


fig = plt.figure(figsize=(4, 3))
ax = fig.gca(projection="3d")
# ax.quiver(0, 0, 0, 1, 0, 0, length=2, normalize=True, color=(0, 0, 1, 1))
# ax.quiver(0, 0, 0, 0, 1, 0, length=2, normalize=True)
# ax.quiver(0, 0, 0, 0, 0, 1, length=2, normalize=True)
arrow(ax, T, 1)
ax.set_xlim(0, 5)
ax.set_ylim(0, 5)
ax.set_zlim(0, 5)

