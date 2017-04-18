#!/bin/bash
# This piece of code just prints all results all together.

bin_count=20

for i in {1..5}; do
  for criterion in AIC BIC; do
    for query_number in {1..50}; do 
      cache_dir="/var/tmp/caches/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/train/query${query_number}/"
      
      # If the cache for the current query number exists...
      if [ -d "$cache_dir" ]; then
        out_path="results/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/query${query_number}"
        
        tail -n+2 $out_path/results
      fi
    done
  done
done
