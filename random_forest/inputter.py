# Inputter treats the dataset and returns a list of rows

from base.row import Row
from base.feature import Feature


class Inputter:
  '''
    Class that parses the classifier's input.
  '''

  def parse(self, data):
    '''
      The method parse receives a list of string.
      Each string corresponds to a row of the dataset.
      The method parses the data and outputs a list of rows.
    '''

    result = []
    for point in data:
      result.append(Row(point))
    return result
