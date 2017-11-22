#!/usr/bin/env python

import subprocess
import glob

p = subprocess.Popen(['sac'], 
                     stdout = subprocess.PIPE,  
                     stdin  = subprocess.PIPE, 
                     stderr = subprocess.STDOUT )

s = "echo on\n"
for filename in glob.glob("*.sac"):
    s += '''
       read %(file)s
       rmean 
       rtrend
       lp co 0.1 p 2 n 4
       write %(file)s.filtered
     ''' % ( {'file': filename } )
s += "quit\n"
out = p.communicate( s.encode('ascii') )
print(out[0].decode())

