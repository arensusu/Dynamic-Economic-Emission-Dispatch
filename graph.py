import numpy as np
import matplotlib.pyplot as plt
import os

INDEX = 2

x = [[40000, 60000], [2450000, 3000000], [650000, 750000]]
y = [[15000, 30000], [290000, 400000], [300000, 500000]]

probList = ["5_24_WOB", "10_24_WOB", "15_24_WOB", "40_24_N"]

pname = './Output/'

for prob in probList :
  try :
    data = open(pname + prob + '/0.igd', 'r')
  except IOError :
    continue
  
  #xAxis = x[INDEX]
  #yAxis = y[INDEX]

  #if not os.path.exists(pname) :
  #  os.mkdir(pname)

  igd = []
  #costs = []
  #emissions = []

  for line in data.readlines() :
    lis = line.strip('\n')
    igd += [float(lis)]

    """
    else :
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
    """

  plt.plot(np.array(igd[:]))
  plt.xlabel('Iteration')
  plt.ylabel('IGD')

  plt.savefig(pname + prob + '/igd.png', dpi = 600)
  print(pname + prob + " finish")

  plt.clf()

  data.close()
