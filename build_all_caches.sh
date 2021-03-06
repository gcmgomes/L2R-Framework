#!/bin/bash

script_path="bazel-bin/bayes/branch_and_bound/binary_sources/cache_builder"

# Compiling the per query cache builder.
bazel build bayes/branch_and_bound/binary_sources:cache_builder -c opt

bin_count=20

echo "Running..."
for i in {1..5}; do
  in_path="datasets/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold$i"
  out_aic_path="/var/tmp/caches/AIC/2003_td_dataset_discretized_${bin_count}_bins_uniform/allqueries/Fold$i"
  out_bic_path="/var/tmp/caches/BIC/2003_td_dataset_discretized_${bin_count}_bins_uniform/allqueries/Fold$i"
  queue_tmp_aic_path="/var/tmp/queue_tmp/AIC/allqueries/Fold$i"
  queue_tmp_bic_path="/var/tmp/queue_tmp/BIC/allqueries/Fold$i"
  
# Input style of the per query cache builder...
# [input file] [cache directory] [memory cache size] [temporary queue directory] [memory queue size] [bin count] [criterion (0 -> AIC), (1 -> BIC)]

  echo "Running Fold $i..."
  mkdir -p log/AIC
  mkdir -p log/BIC

  # Running for AIC
#  echo "./$script_path $in_path/test.txt $out_aic_path/tst 100000 $queue_tmp_aic_path/tst 1000000 ${bin_count} 0 &> log/t${i}_tst &"
#  ./$script_path $in_path/test.txt $out_aic_path/tst 100000 $queue_tmp_aic_path/tst 1000000 ${bin_count} 0 &> log/AIC/t${i}_tst &
  
  echo "./$script_path $in_path/train.txt $out_aic_path/train 100000 $queue_tmp_aic_path/train 1000000 ${bin_count} 0 &> log/t${i}_train &"
  ./$script_path $in_path/train.txt $out_aic_path/train 100000 $queue_tmp_aic_path/train 1000000 ${bin_count} 0 &> log/AIC/t${i}_train &
  
#  echo "./$script_path $in_path/vali.txt $out_aic_path/vali 100000 $queue_tmp_aic_path/vali 1000000 ${bin_count} 0 &> log/t${i}_vali"
#  ./$script_path $in_path/vali.txt $out_aic_path/vali 100000 $queue_tmp_aic_path/vali 1000000 ${bin_count} 0 &> log/AIC/t${i}_vali
  

  # Running for BIC
#  ./$script_path $in_path/test.txt $out_bic_path/tst 100000 $queue_tmp_bic_path/tst 1000000 ${bin_count} 1 &> log/BIC/t${i}_tst
  ./$script_path $in_path/train.txt $out_bic_path/train 100000 $queue_tmp_bic_path/train 1000000 ${bin_count} 1 &> log/BIC/t${i}_train
#  ./$script_path $in_path/vali.txt $out_bic_path/vali 100000 $queue_tmp_bic_path/vali 1000000 ${bin_count} 1 &> log/BIC/t${i}_vali
done
