#!/usr/bin/env python
# coding: utf-8
# Plotting the MAP X Score.
# MAP on the Y axis.
# Score on the X axis.

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats.stats import pearsonr
import os.path

bin_count=20

for i in range(5):
  for criterion in ["AIC", "BIC"]:
    for query_number in range(50):
      filename="results/" + criterion + "/2003_td_dataset_discretized_" + str(bin_count) + "_bins_uniform/Fold" + str(i) + "/query" + str(query_number) + "/map_score_ensemble_ps.txt"
      
      
      if os.path.isfile(filename):
        f = open(filename)
        x = []
        y = []
        mini = 12367875
        maxi = -34567898
        for line in f.readlines():
          line = line.split()
          y.append(float(line[0]))
          x.append(float(line[2]))
          maxi = max(maxi, float(line[2]))
          mini = min(mini, float(line[2]))
        f.close()
        
        
        if len(x) == 1:
          continue      
        
        print filename
        print "Fold " + str(i) + " " + criterion + " Query: " + str(query_number) + " #ParentSets: " + str(len(x))
        
        for j in range(len(x)):
          x[j] = (x[j]-mini)/(maxi-mini)
        
        print pearsonr(x, y)
        plt.plot(x, y, "o")
        plt.show()
        print ""
