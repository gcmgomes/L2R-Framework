#!/bin/bash

script_path="bazel-bin/binary_sources/discretize_dataset"


echo "Running..."
for i in {1..5}; do
  in_path="datasets/2003_td_dataset/Fold$i"
  out_path="datasets/2003_td_dataset_discretized_10_bins_uniform/Fold$i"
  
  mkdir -p $out_path
  echo "Running Fold $i..."
  ./$script_path $in_path/test.txt $out_path/test.txt 0 10
  ./$script_path $in_path/train.txt $out_path/train.txt 0 10
  ./$script_path $in_path/vali.txt $out_path/vali.txt 0 10
done

for i in {1..5}; do
  in_path="datasets/2003_td_dataset/Fold$i"
  out_path="datasets/2003_td_dataset_discretized_20_bins_uniform/Fold$i"
  
  mkdir -p $out_path
  echo "Running Fold $i..."
  ./$script_path $in_path/test.txt $out_path/test.txt 0 20
  ./$script_path $in_path/train.txt $out_path/train.txt 0 20
  ./$script_path $in_path/vali.txt $out_path/vali.txt 0 20
done

echo "Running..."
for i in {1..5}; do
  in_path="datasets/2003_td_dataset/Fold$i"
  out_path="datasets/2003_td_dataset_discretized_10_bins_tube/Fold$i"
  
  mkdir -p $out_path
  echo "Running Fold $i..."
  ./$script_path $in_path/test.txt $out_path/test.txt 1 10
  ./$script_path $in_path/train.txt $out_path/train.txt 1 10
  ./$script_path $in_path/vali.txt $out_path/vali.txt 1 10
done
