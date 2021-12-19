import numpy as np
import matplotlib.pyplot as plt
import os

x = [[40000, 60000], [2500000, 2800000], [650000, 750000]]
y = [[15000, 30000], [300000, 350000], [300000, 500000]]

probList = ["5_24_WOB", "10_24_WOB", "15_24_WOB", "30_24_N", "40_24_N"]
pname = './Output/0.5_0.5_fin_poly/'

INDEX = 1
prob = probList[INDEX]

data = open(pname + prob + '/0.trend', 'r')
  
xAxis = x[INDEX]
yAxis = y[INDEX]

if not os.path.exists(pname + "FIG/") :
  os.mkdir(pname + "FIG/")

igd = []
costs = []
emissions = []

data.readline()
i = 0
for line in data.readlines() :
    if i % 3 == 0 :
      for tup in line.strip('\n').split(', ') :
        lis = tup.strip('()').split(' ')
        if len(lis) != 3 :
          continue

        costs += [float(lis[0])]
        emissions += [float(lis[1])]

      plt.scatter(np.array(costs), np.array(emissions), marker = '.')
      plt.xlim(xAxis[0], xAxis[1])
      plt.ylim(yAxis[0], yAxis[1])
      plt.xlabel('Cost')
      plt.ylabel('Emission')

      if i % 3 == 0:
        plt.savefig(pname + "FIG/" + str(i // 3) + '.png', dpi = 600)

      plt.clf()

      emissions.clear()
      costs.clear()

    i += 1

print(pname + prob + " finish")

plt.clf()

data.close()
