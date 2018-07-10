# Much of this code is due to J. Arias de Reyna and J. van de Lune.                            
# It was modified for the purposes of the paper "On Algorithms to Calculate                   
# Integer Complexity" by Cordwell, Epstein, Hemmady, Miller, Palsson, Sharma, Steinerberger, and
# Yen Nhi Truong Vu. Our work closely follows "Algorithms for Determining Integer Complexity" 
# by Arias de Reyna and van de Lune.   

# This code tests how often our methods can improve AV's work with a very optimistic
# uniform bound.  See Section 4 of our work for the details.

import sys
sys.path.insert(0,'/Volumes/RIEMANN/Todo/Matematicas/Python')
from A000792 import *
from math import ceil

from math import *
nMax = 2000000
# stores the potential number improved
pot_num_imp = 0


cMax = int(ceil(3.*log(nMax)/log(2)))
print 'cMax = ', cMax
# initialize the dictionary so that Compl[n] is always >= the true complexity of n
Compl={}
for n in range(2,nMax+1):
    Compl[n]=cMax
Compl[1]=1

for n in range(2,nMax+1):
    # Compute limitm, which is Arias de Reyna's and van de Lune's bound on the number of     
    # summands that must be checked to calculate the complexity of n.
    a = Compl[n-1]+1
    if a < Compl[n]:
        Compl[n]=a  # the usual best value
    target = Compl[n-1]
    k=target/2
    while (A000792(k)+A000792(target-k)<n):
        k=k-1
    limitm = A000792(k)

    # Introduce whatever optimistic bound you would like to test here
    opt_bound = float(3.3)/3 - 1
    up_to = int(ceil(n**opt_bound))
    if limitm > up_to:
        pot_num_imp = pot_num_imp + 1

    # Calculate complexities                                                                 
    # use the better bound when testing sums       
    for m in range(6,limitm+1):
        sumvalue = Compl[m]+Compl[n-m]
        if sumvalue < Compl[n]:
            Compl[n] = sumvalue
    # test products
    for k in range(2, min(n,nMax/n)+1):
        prodvalue = Compl[k]+Compl[n]
        if prodvalue <Compl[k*n]:
            Compl[k*n]=prodvalue
    # end of computation of complexities
print "Computed complexities upto nMax Compl[",nMax,"] = ", Compl[nMax]
print "Potential num improved is", pot_num_imp

"""
    # report the first value. For example
first =0
for n in range(1,nMax+1):
    if Compl[n]>first:
        first = Compl[n]
        print 'First value of complexity ', Compl[n], '  taken for n = ', n
"""
