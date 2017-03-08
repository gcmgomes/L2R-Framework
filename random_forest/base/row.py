from feature import Feature


class Row:
  '''
    The class row represents a point of a dataset.
  '''

  query_id_ = -1
  doc_id_ = -1
  label_ = -1
  feature_set_ = []

  def __init__(self, line=""):
    if len(line) > 0:
      self.initialize(line)

  def initialize(self, line):
    '''
      The method initialize parses a row from the dataset passed as input.
      It follows the schema used at TD2003/TD2004.
    '''
    self.feature_set_ = []
    splitted = line.split()
    for field in splitted:
      if field.find('#') != -1:
        # Docid
        self.doc_id_ = int(field[field.find('#')+3:])
      elif field.find('qid') != -1:
        # Queryid
        self.query_id_ = int(field[field.find('qid')+4:])
      elif field.find(':') != -1:
        # feature
        feature_id = int(field[:field.find(':')])
        feature_value = float(field[field.find(':')+1:])
        self.feature_set_.append(Feature(feature_id, feature_value))
      else:
        # label
        self.label_ = int(field)


  def doc_id(self):
    return self.doc_id_
  
  
  def query_id(self):
    return self.query_id_
  

  def label(self):
    return self.label_
  

  def feature_set(self):
    return self.feature_set_
