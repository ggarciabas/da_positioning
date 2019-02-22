# -*- coding: UTF-8 -*-
# libraries and data
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import glob
import os
import sys


file = open(sys.argv[1], 'r')
line = file.readline().strip()
N = [x for x in line.split(',')]
data = []
for line in file:
    val = line[:-1].split(',')
    d = [float(x) for x in val[:-1]]
    if len(d) > 0:
        data.append(d[:])
file.close()

data = np.array(data[:])
print data

plt.plot(data[-1])
plt.show()