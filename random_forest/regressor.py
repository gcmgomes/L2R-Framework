from inputter import Inputter
from sklearn.ensemble import RandomForestRegressor


class Regressor:
  '''
    The Regressor class can train a random forest and:
      * Classify new data.
      * Print the Mean Average Precision given a test dataset.
  '''

  # A list of rows (base/row.py).
  parsed_train_ = None

  # The Random Forest Object.
  clf_ = None

  def __init__(self):
    pass


  def load_train_data(self, train_filename):
    '''
      The load_train_data method loads and parses the dataset found
      at the file stored at [train_filename].
    ''' 
    train_file = open(train_filename)
    train_file_content = train_file.readlines()
    inp = Inputter()
    self.parsed_train_ = inp.parse(train_file_content)
    train_file.close()
  
  
  def train(self, number_trees=10):
    '''
      The train method fits the random forest using the train data loaded by
      load_train_data and with [number_trees] decision trees.
    '''
    
    if self.parsed_train_ == None:
      raise Exception('Error: The train file was not loaded.')
    
    # Initializing the random forest object.
    self.clf_ = RandomForestRegressor(n_estimators=number_trees, n_jobs=3, oob_score=True)
    
    X = []
    Y = []
    
    # Loading X and Y...
    for row in self.parsed_train_:
      Y.append(row.label())
      feature_set = row.feature_set()
      features = []    
      
      for feature in feature_set:
        features.append(feature.value())
      X.append(features)
    
    self.clf_.fit(X, Y) 
