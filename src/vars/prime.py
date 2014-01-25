#!/usr/bin/python

# Find prime numbers closest half way between successive powers of two

def isprime(n):
    n*=1.0
    if n%2==0 and n!=2 or n%3==0 and n!=3:
        return False
    for b in range(1,int((n**0.5+1)/6.0+1)):
        if n%(6*b-1)==0:
            return False
        if n %(6*b+1)==0:
           return False
    return True

n = 2
for n in range(1,23) :
    opt = (2**(n+1)+2**n)/2.0
    x = [0,1]
    for a in range(2**n,2**(n+1)):
        if isprime(a):
            dm = abs((a-opt)/(2**n))
            if dm < x[1]: 
                x = [a,dm]
            #print 2**n,2**(n+1),dm,a,x
    print x[0]
    
