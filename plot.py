#!/usr/bin/env python
# coding: utf-8
# Plotting the MAP X Score.
# MAP on the Y axis.
# Score on the X axis.

import numpy as np
import matplotlib.pyplot as plt
import plotly.plotly as py
from scipy.stats.stats import pearsonr
import os.path

bin_count=20

for i in range(5):
  for criterion in ["AIC", "BIC"]:
    for query_number in range(50):
      filename="results/" + criterion + "/2003_td_dataset_discretized_" + str(bin_count) + "_bins_uniform/Fold" + str(i) + "/query" + str(query_number)
      if os.path.isfile(filename):
        print "Fold " + str(i) + " " + criterion + " Query: " + str(query_number)
        f = open(filename)
        x = []
        y = []
        for line in f.readlines():
          line = line.split()
          y.append(float(line[0]))
          x.append(float(line[2]))
        
        print pearsonr(x, y)
        plt.plot(x, y, "o")
