#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

data = np.sin(np.arange(16)*2*np.pi/(16.0 / 2))
#data = np.zeros(16)
#data[1] = 1.0
#data[4] = 1.0
#data[7] = 1.0
#plt.plot(data)
#plt.show()

z = np.fft.fft(data)
for i,zi in enumerate(z):
    print("%3d %15e %15e" % (i,zi.real,zi.imag))

#print(z)
#n = int(z.shape[0])
#for i in range(1,n//2):
#    print(i,z[i].imag + z[n-i].imag, z[i].real - z[n-i].real)
