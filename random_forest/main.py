#!/usr/bin/env python
# coding: utf-8
#
# This snippet of code receives train and test data, generates a random forest
# and prints the Mean Average Precision.


import sys
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

  total = 0
  for i in range(30):
    random_forest.train(500)
    total += random_forest.mapk(10)
  print "MAP@10:", total/30


if __name__ == '__main__':
  main()
