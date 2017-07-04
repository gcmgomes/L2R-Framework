#!/bin/bash

script_path="bazel-bin/bayes/branch_and_bound/binary_sources/simple_ps_ranker"


echo "Compiling..."
bazel build bayes/branch_and_bound/binary_sources/simple_ps_ranker -c opt


echo "Running..."
# Arguments of simple_ps_ranker
# [train filename] [query number -> -1 if all queries] [test filename] [label cache directory] [output path] [criterion (0 -> AIC), (1 -> BIC)]

for bin_count in {10,20,30}; do
  for i in {1..5}; do
    echo "Running Fold $i..."
    for criterion in AIC BIC; do
      echo "For Criterion ${criterion}"
      for query_number in {1..50}; do
        cache_dir="/var/tmp/caches/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/train/query${query_number}/"
        
        # If the cache for the current query number exists...
        if [ -d "$cache_dir" ]; then
          in_path="datasets/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold$i"
          out_path="results/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/query${query_number}"
          
          criterion_id=0
          if [ "$criterion" == "BIC" ]; then
            criterion_id=1
          fi
          
          echo "./$script_path $in_path/train.txt ${query_number} $in_path/test.txt $cache_dir $out_path $criterion_id"
          ./$script_path $in_path/train.txt ${query_number} $in_path/test.txt $cache_dir $out_path $criterion_id
        fi
      done
      echo ""
    done
  done
done
