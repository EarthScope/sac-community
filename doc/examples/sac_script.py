#!/usr/bin/env python

import subprocess

p = subprocess.Popen(['sac'], 
                     stdout = subprocess.PIPE,  
                     stdin  = subprocess.PIPE, 
                     stderr = subprocess.STDOUT )

out = p.communicate('''
fg seismo
lh columns 2
quit
'''.encode('ascii'))

print(out[0].decode())



