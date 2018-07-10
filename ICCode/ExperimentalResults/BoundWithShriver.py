# Much of this code is due to J. Arias de Reyna and J. van de Lune.                            
# It was modified for the purposes of the paper "On Algorithms to Calculate                    
# Integer Complexity" by Cordwell, Epstein, Hemmady, Miller, Palsson, Sharma, Steinerberger, and
# Yen Nhi Truong Vu. Our work closely follows "Algorithms for Determining Integer Complexity"  
# by Arias de Reyna and van de Lune.                                                            

# We introduce a precomputation and check how often the precomputation yields an improved      
# summand bound. (See Section 4 in "On Algorithms to Calculate Integer Complexity".)            
# Here we use a combination of Steinerberger's Markov chain algorithm and results from Shriver's greedy algorithm in base 2310 in the precomputation.       

# Run this code with Python 2.7.

#from __future__ import division
import sys
sys.path.insert(0,'/Volumes/RIEMANN/Todo/Matematicas/Python')
from A000792 import *
from fractions import Fraction
from math import ceil
from math import log
from math import floor
from math import *

markov_dict = {}

# Steinerberger's Markov chain algorithm (see "A short note on integer complexity") 
def markovbound(n):
    count = 0
    if n <= 5:
        count = n
    else:
        if n%6 == 0 or n%6 == 3:
            count = 3 + markov_dict[n/3]
        if n%6 == 1:
            count = 4 + markov_dict[(n-1)/3]
        if n%6 == 2 or n%6 == 4:
            count = 2 + markov_dict[n/2]
        if n%6 == 5:
            count = 3 + markov_dict[(n-1)/2]
    markov_dict[n] = count

for i in range (1, 2000001):
    markovbound(i)

# We store results from Shriver's greedy algorithm run in base 2310 in the file "ShriverGreedyAlgResults.txt"
def shriver_markov_bound():
    file = open('ShriverGreedyAlgResults.txt', 'r')
    for line in file:
        # lines are of the format n f(n) where f(n) comes from Shriver's 
        # greedy alg run in base 2310
        shriver_arr = line.split(' ')
        # Leave the small values unchanged
        if int(shriver_arr[0]) > 5 and int(shriver_arr[0]) < 2000001:
            # update dict values when Shriver's algorithm gives better results than Steinerberger's
            if markov_dict[int(shriver_arr[0])] > int(shriver_arr[1]):
                markov_dict[int(shriver_arr[0])] = int(shriver_arr[1])
    file.close()
shriver_markov_bound()

# Use the reasoning in "On Algorithms ..." to calculate summand bounds.
def precompute_summands(n):
    c = markov_dict[n]
    # Make sure to never round down                                   
    log_bound = c/float(log(n, 3)) + 1/float(100000000000)
    bound = float(log_bound)/3 - 1
    return bound

# This method tests how often our summand bounds are better than Arias de Reyna and            
# van de Lune's summand bounds.
def compute_complexities(num):
    # numImproved counts how often the new summand bound kicks in
    numImproved = 0
    nMax = num

    cMax = int(ceil(3.*log(nMax)/log(2)))
    print 'cMax = ', cMax

    # initialize the dictionary so that Compl[n] is always >= the true complexity 
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

        # up_to is our bound on the number of summands to check.  We compare them here.
        markov_sumbound = precompute_summands(n) 
        up_to = int(ceil(n**markov_sumbound))
        if (up_to < limitm):
            numImproved += 1
        limitm = min(limitm, up_to)

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
    print "Num improved ", numImproved
"""
    # report the first value. For example
first =0
for n in range(1,nMax+1):
    if Compl[n]>first:
        first = Compl[n]
        print 'First value of complexity ', Compl[n], '  taken for n = ', n
"""

compute_complexities(2000000)
