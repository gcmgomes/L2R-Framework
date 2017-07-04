#!/bin/bash

script_path="bazel-bin/binary_sources/discretize_dataset"

echo "Running..."
for bins in {10,20,30}; do
  for i in {1..5}; do
    in_path="datasets/2003_td_dataset/Fold$i"
    out_path="datasets/2003_td_dataset_discretized_${bins}_bins_uniform/Fold$i"
    
    mkdir -p $out_path
    echo "Running Fold $i..."
    ./$script_path $in_path/test.txt $out_path/test.txt 0 ${bins}
    ./$script_path $in_path/train.txt $out_path/train.txt 0 ${bins}
    ./$script_path $in_path/vali.txt $out_path/vali.txt 0 ${bins}
  done
  
  
  for i in {1..5}; do
    in_path="datasets/2003_td_dataset/Fold$i"
    out_path="datasets/2003_td_dataset_discretized_${bins}_bins_tube/Fold$i"
    
    mkdir -p $out_path
    echo "Running Fold $i..."
    ./$script_path $in_path/test.txt $out_path/test.txt 1 ${bins}
    ./$script_path $in_path/train.txt $out_path/train.txt 1 ${bins}
    ./$script_path $in_path/vali.txt $out_path/vali.txt 1 ${bins}
  done

done
