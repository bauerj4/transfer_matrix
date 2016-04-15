import numpy as np
import matplotlib.pylab as plt

dt = np.dtype(float)

temp = np.fromfile("OUTPUTS/temperature.bin", dtype=dt)
beta = np.fromfile("OUTPUTS/beta.bin", dtype=dt)
part = np.fromfile("OUTPUTS/partition.bin", dtype=dt)

dbeta = np.gradient(beta)
dT = np.gradient(temp)

print dT, dbeta

Energy = np.gradient(-part, dbeta)#, edge_order=2)
HeatCapacity = -np.gradient(Energy, dbeta)

plt.plot(temp, Energy)
plt.show()

plt.plot(temp[1:len(temp)-2], HeatCapacity[1:len(HeatCapacity)-2])
plt.show()
