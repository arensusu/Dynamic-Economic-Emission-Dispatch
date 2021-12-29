import numpy as np
import matplotlib.pyplot as plt
import os

INDEX = 2

x = [[40000, 60000], [2500000, 2800000], [650000, 750000]]
y = [[15000, 30000], [300000, 350000], [300000, 500000]]

probList = ["5_24_WOB", "10_24_WOB", "15_24_WOB", "30_24_N", "40_24_N"]

pname1 = './Output/0.5_0.5_fin/'
pname2 = './Output/0.5_0.5_fin_poly/'

for prob in probList :
  try :
    data1 = open(pname1 + prob + '/0.trend', 'r')
    data2 = open(pname2 + prob + "/0.trend", 'r')
  except IOError :
    continue

  igd = []

  i = 0
  for line in data1.readlines() :
    if i % 3 == 2 :
      lis = line.strip('\n').split(' ')
      igd += [float(lis[2])]

    i += 1

  plt.plot(np.array(igd[200:]))

  igd = []
  i = 0
  for line in data2.readlines() :
    if i % 3 == 2 :
      lis = line.strip('\n').split(' ')
      igd += [float(lis[2]), float(lis[2])]

    i += 1

  plt.plot(np.array(igd[200:]))
  plt.xlabel('Iteration')
  plt.ylabel('IGD')

  plt.savefig("./Output/compare/" + prob + '.png', dpi = 600)
  print(prob + " finish")

  plt.clf()

  data1.close()
  data2.close()
