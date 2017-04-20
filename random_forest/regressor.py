from inputter import Inputter
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import average_precision_score

class Regressor:
  '''
    The Regressor class can train a random forest and:
      * Classify new data.
      * Print the Mean Average Precision given a test dataset.
  '''

  # A list of rows (base/row.py).
  parsed_train_ = None
  
  # A list of rows (base/row.py).
  parsed_test_ = None

  # The Random Forest Object.
  clf_ = None

  def __init__(self):
    pass
  

  def load_dataset(self, filename):
    '''
      The load_dataset method loads and parses the dataset found
      at the file stored at [filename].
    ''' 
    fileb = open(filename)
    file_content = fileb.readlines()
    inp = Inputter()
    parsed_dataset = inp.parse(file_content)
    fileb.close()
    return parsed_dataset


  def load_train_data(self, train_filename):
    '''
      The load_train_data method loads and parses the dataset found
      at the file stored at [train_filename].
    ''' 
    self.parsed_train_ = self.load_dataset(train_filename)


  def load_test_data(self, test_filename):
    '''
      The load_test_data method loads and parses the dataset found
      at the file stored at [test_filename].
    ''' 
    self.parsed_test_ = self.load_dataset(test_filename)

  
  def get_train_size(self):
    '''
      Returns the number of rows train has.
    '''
    return len(self.parsed_train_)

  
  def get_test_size(self):
    '''
      Returns the number of rows test has.
    '''
    return len(self.parsed_test_)
  

  def toXY(self, parsed_dataset):
    '''
      toXY takes a parsed dataset and transforms it to two arrays,
      X and Y, being X the classifier input and Y the output.
    '''
    X = []
    Y = []
    
    # Loading X and Y...
    for row in parsed_dataset:
      Y.append(row.label())
      feature_set = row.feature_set()
      features = []    
      
      for feature in feature_set:
        features.append(feature.value())
      X.append(features)
    return [X, Y]

  
  def train(self, number_trees=10):
    '''
      The train method fits the random forest using the train data loaded by
      load_train_data and with [number_trees] decision trees.
    '''
    
    if self.parsed_train_ == None:
      raise Exception('Error: The train file was not loaded.')
    
    # Initializing the random forest object.
    self.clf_ = RandomForestRegressor(n_estimators=number_trees, n_jobs=4, oob_score=True)
    
    transformed_dataset = self.toXY(self.parsed_train_)
    X = transformed_dataset[0]
    Y = transformed_dataset[1]
    self.clf_.fit(X, Y) 
  

  def print_score(self):
    '''
      Prints the predictions for each document.
    '''
    transformed_dataset = self.toXY(self.parsed_test_)
    X = transformed_dataset[0]
    predictions = self.clf_.predict(X)
    
    for value in predictions:
      print value
