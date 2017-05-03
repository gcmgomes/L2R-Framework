#!/bin/bash


bin_count=20

for i in {1..5}; do
  for criterion in AIC BIC; do
    in_path="datasets/2003_td_dataset/Fold$i"
    rf_path="random_forest/predictions/discretized_20_bins_uniform"
    perl Eval-Score-3.0.pl $in_path/test.txt ${rf_path}/fold${i} ${rf_path}/rf_mapzinho 0
    echo "Fold$i $(grep MAP ${rf_path}/rf_mapzinho)"
    
    for query_number in {1..50}; do
      cache_dir="/var/tmp/caches/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/train/query${query_number}/"
      
      # If the cache for the current query number exists...
      if [ -d "$cache_dir" ]; then
        echo "  Query $query_number"
        out_path="results/${criterion}/2003_td_dataset_discretized_${bin_count}_bins_uniform/Fold${i}/query${query_number}"
        if [ -f "${out_path}/map_score.txt" ]; then
          rm ${out_path}/map_score.txt
        fi

        if [ -f "${out_path}/map_score_simple_ps.txt" ]; then
          rm ${out_path}/map_score_simple_ps.txt
        fi
        
        if [ -f "${out_path}/map_score_ensemble_ps.txt" ]; then
          rm ${out_path}/map_score_ensemble_ps.txt
        fi
        
        criterion_id=0
        if [ "$criterion" == "BIC" ]; then
          criterion_id=1
        fi
        
        
        for ps in {1..1000}; do
          map_output="${out_path}/simple_ps_results_${ps}"
          if [ -f "$map_output" ]; then
            head -n -1 $map_output &> tmp_result
            perl Eval-Score-3.0.pl $in_path/test.txt tmp_result ${out_path}/mapzinho_simple_${ps} 0
            echo $(grep MAP ${out_path}/mapzinho_simple_${ps} | grep -o -E "[0-9]\.[0-9]+") $(tail -n 1 $map_output) >> ${out_path}/map_score_simple_ps.txt
          else
            break
          fi
        done
        
        for ps in {1..1000}; do
          map_output="${out_path}/ensemble_ps_results_random_${ps}"
          if [ -f "$map_output" ]; then
            head -n -1 $map_output &> tmp_result
            perl Eval-Score-3.0.pl $in_path/test.txt tmp_result ${out_path}/mapzinho_ensemble_${ps} 0
            echo $(grep MAP ${out_path}/mapzinho_ensemble_${ps} | grep -o -E "[0-9]\.[0-9]+") $(tail -n 1 $map_output) >> ${out_path}/map_score_ensemble_ps.txt
          else
            break
          fi
          
          
          map_output="${out_path}/ensemble_ps_results_minimum_0"
          perl Eval-Score-3.0.pl $in_path/test.txt tmp_result ${out_path}/mapzinho_minimum_0 0
          echo $(grep MAP ${out_path}/mapzinho_minimum_0 | grep -o -E "[0-9]\.[0-9]+") $(tail -n 1 $map_output) >> ${out_path}/map_score_ensemble_ps.txt


          map_output="${out_path}/ensemble_ps_results_maximum_0"
          perl Eval-Score-3.0.pl $in_path/test.txt tmp_result ${out_path}/mapzinho_maximum_0 0
          echo $(grep MAP ${out_path}/mapzinho_maximum_0 | grep -o -E "[0-9]\.[0-9]+") $(tail -n 1 $map_output) >> ${out_path}/map_score_ensemble_ps.txt
        done
        
      fi
    done
  done
done
