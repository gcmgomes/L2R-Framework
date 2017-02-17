#!/usr/bin/env python
# coding: utf-8
#
# This snippet of code receives train and test data, generates a random forest
# and prints the Mean Average Precision.


import sys
from classifier import Classifier


def usage():
  '''
    The method usage returns a message explaining how to run the classifier.
  '''
  return "Usage:\npython " + sys.argv[0] + " [train filename] [test filename]"


def main():
  if len(sys.argv) != 3:
    print usage()
    return
  
  random_forest = Classifier()
  random_forest.load_train_data(sys.argv[1])
  random_forest.train(300)


if __name__ == '__main__':
  main()
