import numpy as np
import matplotlib.pyplot as plt
import os

pname = './Output/5_24_FIG'

data = open('./Output/5_24_WOB/0.trend', 'r')

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
  plt.xlim(0, 100000)
  plt.ylim(0, 100000)
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
