#!/bin/bash

script_path="util/binary_sources/to_dat_converter.py"

echo "Running original..."
for i in {1..5}; do
  path="datasets/2003_td_dataset/Fold$i"
  echo "Running Fold $i..."
  python $script_path $path/test.txt $path/test.dat
  python $script_path $path/train.txt $path/train.dat
  python $script_path $path/vali.txt $path/vali.dat
done

echo ""
echo "Running discretized tube 10 bins..."
for i in {1..5}; do
  path="datasets/2003_td_dataset_discretized_10_bins_tube/Fold$i"
  echo "Running Fold $i..."
  python $script_path $path/test.txt $path/test.dat
  python $script_path $path/train.txt $path/train.dat
  python $script_path $path/vali.txt $path/vali.dat
done

echo ""
echo "Running discretized uniform 20 bins..."
for i in {1..5}; do
  path="datasets/2003_td_dataset_discretized_20_bins_uniform/Fold$i"
  echo "Running Fold $i..."
  python $script_path $path/test.txt $path/test.dat
  python $script_path $path/train.txt $path/train.dat
  python $script_path $path/vali.txt $path/vali.dat
done
