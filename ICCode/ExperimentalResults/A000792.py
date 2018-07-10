# This file is from Prof. Arias de Reyna and Prof van de Lune's code.  It is used in "BoundWithShriver.py", "BoundWithSteinerberger.py", and "TestOptimisticUniformBound.py".

# Sloane A000792
# A000792(n) is maximum number with complexity n

def A000792(n):
    n= n
    result =1
    while (n>=5)or(n==3):
        result *= 3
        n -= 3
    return result * (2**(n/2))
    



