#!/bin/bash

script_path="random_forest/main.py"

echo "Running original..."
for i in {1..5}; do
  path="datasets/2003_td_dataset/Fold$i"
  echo "Running Fold $i..."
  python $script_path $path/train.txt $path/test.txt
  echo ""
done

echo ""
echo ""
echo "Running discretized tube 10 bins..."
for i in {1..5}; do
  path="datasets/2003_td_dataset_discretized_10_bins_tube/Fold$i"
  echo "Running Fold $i..."
  python $script_path $path/train.txt $path/test.txt
  echo ""
done
