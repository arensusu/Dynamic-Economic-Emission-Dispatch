import matplotlib.pyplot as plot
import numpy as np
from scipy.stats import ranksums

arch = "operator"
#algoLists = ["rand1", "rand2", "best1", "random", "improveRate", "improvePercent", "hybrid", "2"]
algoLists = ["rand1", "rate"]
probLists = ["5_24_WOB", "6_24_WOB", "10_24_WOB", "15_24_WOB", "30_24_N", "40_24_N"]
run = 20

out = open("./igdList.csv", "w")

out.write(", " + algoLists[0] + ", , " + algoLists[1] + ", , pvalue\n")
 
for prob in probLists:
    data = []

    out.write(prob)

    for algo in algoLists:
        path = "./Output/" + arch + "/" + algo + "/" + prob + "/"

        igdLists = np.zeros(run)

        for i in range(run):
            with open(path + str(i) + ".igd") as f:
                for line in f.readlines():
                    igdLists[i] = line.strip('\n')

        out.write(", " + str(igdLists.mean()) + ", " + str(igdLists.std()))

        data.append(igdLists)

    _, pvalues = ranksums(data[0], data[1])

    out.write(", " + str(pvalues) + "\n")



out.close()