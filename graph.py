import numpy as np
import matplotlib.pyplot as plt
import copy
import os

INDEX = 5

x = [[40000, 60000], [2450000, 2600000], [650000, 750000]]
y = [[15000, 30000], [290000, 330000], [300000, 500000]]

probSize = ['5', '6', '10', '15', '30', '40']
probList = ["5_24_WOB", "6_24_WOB", "10_24_WOB", "15_24_WOB", "30_24_N", "40_24_N"]

exp = "./Output/MOEADcompare/"

algoName = ['MOEA/D', "MOEA/D-DRA"]
algo1 = exp + 'origin/'
algo2 = exp + 'DRA/'


plt.rc('font', size=16)
plt.figure(figsize=(10, 6))

plt.scatter(c1, e1, marker='x')
plt.scatter(c2, e2, marker='o')
plt.scatter(rf_c, rf_e, marker='.', c='black')

plt.title(probSize[INDEX] + "-U: " + algoName[0] + " vs. " + algoName[1])
plt.xlabel("Cost")
plt.ylabel("Emission")
plt.legend([algoName[0], algoName[1], 'reference'])
plt.xticks(rotation=10)

plt.savefig(probSize[INDEX] + ".png", dpi=100)

plt.clf()