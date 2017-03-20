#!/bin/bash

script_path="bazel-bin/binary_sources/discretize_dataset"

echo "Running..."
for i in {1..5}; do
  in_path="datasets/2003_td_dataset/Fold$i"
  out_path="datasets/2003_td_dataset_discretized_10_bins_tube/Fold$i"
  echo "Running Fold $i..."
  ./$script_path $in_path/test.txt $out_path/test.txt
  ./$script_path $in_path/train.txt $out_path/train.txt
  ./$script_path $in_path/vali.txt $out_path/vali.txt
done
