import myModule
import timeit
from numba import jit


@jit(nopython=True)
def fib(n):
    if n < 2:
        return 1
    else:
        return fib(n-1)+fib(n-2)


print(fib(10)) # the first time calling , let the fib be complied by jit.

myModule.hellworld()
x = myModule.fib(1)

# 1.16s with c-extension
print(timeit.timeit("myModule.fib(32)", "import myModule", number=100))  

# 108.85s without jit, 1.78s with jit
print(timeit.timeit("fib(32)", "from __main__ import fib", number=100))

# conclusion: jit is 1.5x c-extentison version, but pure python version is nearly 100x slower.
