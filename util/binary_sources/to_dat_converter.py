#!/usr/bin/env
# coding: utf-8
# 
# Converts the format of the input file into a DAT format.

import sys


def main():
  if len(sys.argv) != 3:
    print 'Error: Please inform the input file name.'
    print 'Usage: python ' + sys.argv[0] + ' [input filename] [output filename]'
    return
  
  f = open(sys.argv[1], 'r')
  out = open(sys.argv[2], 'w')
  
  all_lines = f.readlines()
 
  # Counting the number of columns... 
  columns=0
  splitted = all_lines[0].split()
  for cell in splitted:
    if cell.find(':') != -1:
      if cell.find('qid') == -1:
        columns+=1
    elif cell.find('#') != -1:
      break
    else:
      columns+=1

  columns-=1
  out.write('Label ')
  for i in range(columns):
    out.write(str(i) + ' ')
  out.write('\n')

  for line in all_lines:
    splitted = line.split()
    for cell in splitted:
      if cell.find(':') != -1:
        if cell.find('qid') == -1:
          out.write(cell[cell.find(':')+1:] + ' ')
      elif cell.find('#') != -1:
        # Ignore the document id.
        break
      else:
        out.write(cell + ' ')
    out.write('\n')
  
  f.close()
  out.close()


if __name__ == '__main__':
  main()
