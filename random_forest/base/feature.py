class Feature:
  '''
    Class feature represents a cell of a feature set.
  '''
  
  feature_id_ = -1
  feature_value_ = -1

  
  def __init__(self, fid=-1, fvalue=-1):
    self.feature_id_ = fid
    self.feature_value_ = fvalue


  def set_id(self, fid):
    self.feature_id_ = fid


  def set_value(self, fvalue):
    self.feature_value_ = fvalue


  def value(self):
    return self.feature_value_


  def id(self):
    return self.feature_id_
