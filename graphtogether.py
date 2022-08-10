import numpy as np
import matplotlib.pyplot as plt
import copy
import os

INDEX = 0

x = [[40000, 60000], [2450000, 2600000], [650000, 750000]]
y = [[15000, 30000], [290000, 330000], [300000, 500000]]

probSize = ['5', '6', '10', '15', '30', '40']
probList = ["5_24_WOB", "6_24_WOB", "10_24_WOB", "15_24_WOB", "30_24_N", "40_24_N"]

exp = "./Output/MOEADcompare/"

algoName = ['MOEA/D', "MOEA/D-DRA"]
algo1 = exp + 'origin/'
algo2 = exp + 'DRA/'

rfPath = "./Indicator/IGD/" + probList[INDEX] + ".igd"

rf_c = []
rf_e = []

with open(rfPath, 'r') as rf:
  num = int(rf.readline().strip('\n'))
  rf.readline()

  for _ in range(num):
    obj = rf.readline().strip('\n').split('\t')
    rf_c.append(float(obj[0]))
    rf_e.append(float(obj[1]))

data1 = open(algo1 + probList[INDEX] + '/3.detail', 'r')
data2 = open(algo2 + probList[INDEX] + "/13.detail", 'r')

targetGeneration = 1000

for i in range(1000):

  c1 = []
  e1 = []
  c2 = []
  e2 = []

  for j in range(100):
    if i % targetGeneration  == targetGeneration - 1:
      ind1 = data1.readline().strip('\n').split(' ')
      ind2 = data2.readline().strip('\n').split(' ')

      c1.append(float(ind1[0]))
      e1.append(float(ind1[1]))
      c2.append(float(ind2[0]))
      e2.append(float(ind2[1]))

    else:
      data1.readline()
      data2.readline()

  if len(c1) != 0:
    plt.rc('font', size=10)
    plt.figure(dpi=600)

    plt.scatter(c1, e1, marker='x')
    plt.scatter(c2, e2, marker='o')
    plt.scatter(rf_c, rf_e, marker='.', c='black')

    plt.title(probSize[INDEX] + "-U: " + algoName[0] + " vs. " + algoName[1])
    plt.xlabel("Cost")
    plt.ylabel("Emission")
    plt.legend([algoName[0], algoName[1], 'reference'])
    plt.xticks(rotation=10)

    plt.savefig(probSize[INDEX] + ".png", dpi=600)

    plt.clf()

    
  data1.readline()
  data2.readline()

data1.close()
data2.close()