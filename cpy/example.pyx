# example.pyx

def factorial(int n):
    cdef int i
    cdef long result = 1
    for i in range(1, n + 1):
        result *= i
    return result
