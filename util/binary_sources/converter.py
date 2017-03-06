#!/usr/bin/env
# coding: urf-8
# 
# Converts the format of the input file into a DAT format.

import sys


def main():
  if len(sys.argv) != 2:
    print 'Error: Please inform the input file name.'
    print 'Usage: python ' + sys.argv[0] + ' [input filename]'
    return
  
  f = open(sys.argv[1], 'r')
  
  for line in f.readlines():
    splitted = line.split()



if __name__ == '__main__':
  main()
