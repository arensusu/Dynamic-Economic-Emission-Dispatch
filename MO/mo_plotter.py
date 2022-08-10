import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import os
from copy import deepcopy


# 動畫參數
StepSize = 5   # 每隔幾代繪製一次
PauseTime = 0.5 # 暫停秒數
FigWidth = 16   # 圖表寬
FigHeight = 10  # 圖表高
AnimationMode = True # False: 批次繪圖
rffname = 'RF.txt' # reference front file name

def main():
    if AnimationMode:
        infname = input('Input the log file name...>')
        plt.ion()
        ProcessFile(infname, rffname)
    else:
        for fname in os.listdir():
            if fname[-4:]!='.txt': continue
            print('Processing', fname)
            ProcessFile(fname, rffname)
            print(GetFigFilename(fname),  'is generated')
        
#-----------------------------------------------------------------------------------------------------------
def ProcessFile(infname, rffname):

    rfront = []
    with open(rffname) as rffile:
        for line in rffile:
            if len(line.split())>0:
                rfront.append([float(e) for e in line.split()])
    
    f1_line_chart = TccChart('generations', 'cost')
    f2_line_chart = TccChart('generations', 'emissions')
    IGD_line_chart = TccChart('generations', 'IGD')

    fig = plt.figure(constrained_layout=True, figsize=(FigWidth, FigHeight))
    gs = GridSpec(4, 2, figure=fig)
    f1_ax = fig.add_subplot(gs[0, 0])
    f2_ax = fig.add_subplot(gs[1, 0])
    IGD_ax = fig.add_subplot(gs[2, 0])
    hmap_ax = fig.add_subplot(gs[0:2, 1])
    scat_ax = fig.add_subplot(gs[2:4, 1])
    
    with open(infname) as infile:
        gen = 0
        pop = []    
        for line in infile:
            if len(line.split()): # not empty line
                indv = [float(e) for e in line.split()]
                pop.append(indv)
            else:
                if gen%StepSize==0:

                    f1_line_chart.Add(gen, pop, 0)
                    f1_line_chart.DrawObjLineChart(f1_ax)
                    f2_line_chart.Add(gen, pop, 1)
                    f2_line_chart.DrawObjLineChart(f2_ax)

                    IGDv = CalcIGD(rfront, pop)
                    IGD_line_chart.AddMetric(gen, IGDv)
                    IGD_line_chart.DrawMetricLineChart(IGD_ax)

                    DrawHeatMap(fig, hmap_ax, pop, lb=0, ub=500)
                    #DrawScatterPlot(scat_ax, pop, (640000, 750000), (300000, 700000))
                    DrawScatterPlot(scat_ax, pop, (2400000, 3200000), (270000, 500000))
                    #DrawScatterPlot(scat_ax, pop, (25000, 29000), (5.5, 7.5))
                    #DrawScatterPlot(scat_ax, pop, (40000, 60000), (15000, 30000))
                    
                    if AnimationMode:
                        plt.pause(PauseTime)

                gen += 1
                pop = []
                
    plt.savefig(GetFigFilename(infname))
#-----------------------------------------------------------------------------------------------------------
class TccChart:
    def __init__(self, xlabel='', ylabel=''):
        self.max_obj = []
        self.mid_obj = []
        self.min_obj = []
        self.metric = []
        self.updated = []
        self.x = []
        self.xlabel = xlabel
        self.ylabel = ylabel

    def Add(self, gen, pop, obj_ind):
        objvec = sorted([indv[obj_ind] for indv in pop])
        if len(self.min_obj)==0 or objvec[0]<self.min_obj[-1]:
            self.updated.append((gen, objvec[0]))
        self.min_obj.append(objvec[0])
        self.mid_obj.append(objvec[len(objvec)//2]) #roughly median
        self.max_obj.append(objvec[-1])
        self.x.append(gen)

    def DrawObjLineChart(self, ax):
        ax.plot(self.x, self.min_obj, marker='o', markersize=2, color='green')
        ax.plot(self.x, self.mid_obj, marker='o', markersize=2, color='gray')
        ax.plot(self.x, self.max_obj, marker='o', markersize=2, color='red')
        ax.plot([e[0] for e in self.updated], [e[1] for e in self.updated], 'o', markersize=5, color='green')
        ax.set_xlabel(self.xlabel)
        ax.set_ylabel(self.ylabel)

    def AddMetric(self, gen, metric):
        if len(self.metric)==0 or metric<self.metric[-1]:
            self.updated.append((gen, metric))
        self.metric.append(metric)
        self.x.append(gen)

    def DrawMetricLineChart(self, ax):
        ax.plot(self.x, self.metric, marker='o', markersize=2, color='blue')
        ax.plot([e[0] for e in self.updated], [e[1] for e in self.updated], 'o', markersize=5, color='cyan')
        ax.set_xlabel(self.xlabel)
        ax.set_ylabel(self.ylabel)

#-----------------------------------------------------------------------------------------------------------
def DrawHeatMap(fig, ax, pop, lb=-1, ub=1):
    ax.clear()
    pop.sort(key=lambda x: x[0])
    genes = [e[2:] for e in pop]

    # Showing text may slow down visualization
##    for i in range(len(genes)):
##        for j in range(len(genes[i])):
##            ax.text(j+0.5, i+0.5, round(genes[i][j],2), color='w', ha='center', va='center', fontsize=6)

    ax.pcolormesh(genes, cmap='seismic', vmin=lb, vmax=ub)
    ax.set_xlabel('genes')
    ax.set_ylabel('individuals')   
#-----------------------------------------------------------------------------------------------------------
def DrawScatterPlot(ax, pop, xlim=(0, 1), ylim=(0, 1)):
    ax.set_xlim(xlim[0], xlim[1])
    ax.set_ylim(ylim[0], ylim[1])

    f1 = [indv[0] for indv in pop]
    f2 = [indv[1] for indv in pop]

    ax.plot(f1, f2, 'o', markersize=3)
#-----------------------------------------------------------------------------------------------------------
def GetFigFilename(infname):
    return 'mo_'+infname[:-4]+'.png'
#-----------------------------------------------------------------------------------------------------------
def CalcIGD(rf_ss, approx_ss):
    def distance(lhs, rhs):
        return ((lhs[0]-rhs[0])**2 + (lhs[1]-rhs[1])**2)**0.5

    rf = deepcopy([e[:2] for e in rf_ss])
    approx = deepcopy([e[:2] for e in approx_ss])

    objs = [[e[0] for e in rf]+[e[0] for e in approx],
            [e[1] for e in rf]+[e[1] for e in approx]
            ]
   
    min_x, max_x = min(objs[0]), max(objs[0])
    min_y, max_y = min(objs[1]), max(objs[1])

    #print(min_x, max_x, min_y, max_y)

    for ss in [rf, approx]:
        for e in ss:
            e[0] = (e[0]-min_x)/(max_x-min_x)
            e[1] = (e[1]-min_y)/(max_y-min_y)
   
    min_dist = []
    for sr in rf:
        min_dist.append(min([distance(sr, sa) for sa in approx]))
    return sum(min_dist)/len(min_dist)
#-----------------------------------------------------------------------------------------------------------



if __name__ == '__main__':
    main()
