#!/usr/bin/env python
# coding: utf-8
#
# This snippet of code receives train and test data, generates a random forest
# and prints the Mean Average Precision.


import sys
from math import sqrt
from regressor import Regressor


def usage():
  '''
    The method usage returns a message explaining how to run the classifier.
  '''
  return "Usage: python " + sys.argv[0] + " [train filename] [test filename]"


def main():
  if len(sys.argv) != 3:
    print usage()
    return
  
  random_forest = Regressor()
  random_forest.load_train_data(sys.argv[1])
  random_forest.load_test_data(sys.argv[2])
  n_tests = 30

  results = []
  
  total = 0.0
  for i in range(n_tests):
    random_forest.train(500)
    results.append(random_forest.mapk(10))
    total += results[-1]
  
  sd = 0
  total = float(total)/n_tests
  for v in results:
    sd += (v-total)*(v-total)
  sd = sqrt(float(sd)/n_tests)
  
  print "MAP@10:", total, " STDDEV: ", sd


if __name__ == '__main__':
  main()
