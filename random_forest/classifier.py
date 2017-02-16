#!/usr/bin/env python
# coding: utf-8
#
# This snippet of code receives train and test data, generates a random forest
# and prints the Mean Average Precision.


import sys
from inputter import Inputter


def usage():
  return "Usage:\npython " + sys.argv[0] + " [train filename] [test filename]"


def main():
  if len(sys.argv) != 3:
    print usage()
    return
  
  train_file = open(sys.argv[1])
  train_file_content = train_file.readlines()
  inp = Inputter()
  train_file_parsed = inp.parse(train_file_content)

  for row in train_file_parsed:
    label = str(row.label())
    doc_id = str(row.doc_id())
    query_id = str(row.query_id())
    feature_set = row.feature_set()
    
    sys.stdout.write(label + " #" + doc_id + " qid:" + query_id + " ")
    for feature in feature_set:
      sys.stdout.write(str(feature.id()) + ":" + str(feature.value()) + " ")
    print ""


if __name__ == '__main__':
  main()
