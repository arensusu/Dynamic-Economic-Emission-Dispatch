import numpy as np
import matplotlib.pyplot as plt
import os

INDEX = 2

x = [[40000, 60000], [2500000, 2800000], [650000, 750000]]
y = [[15000, 30000], [300000, 350000], [300000, 500000]]

pname = './Output/15_24_FIG'

data = open('./Output//0.5_0.5_div/15_24_WOB/0.trend', 'r')

xAxis = x[INDEX]
yAxis = y[INDEX]

if not os.path.exists(pname) :
  os.mkdir(pname)

costs = []
emissions = []

i = 0
for line in data.readlines() :
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
    plt.savefig(pname + '/all_' + str(i // 3) + '.png', dpi = 600)
  #elif i % 3 == 2 :
  #  plt.savefig(pname + '/front_' + str(i // 3) + '.png', dpi = 600)

  plt.clf()

  emissions.clear()
  costs.clear()

  i += 1

data.close()
