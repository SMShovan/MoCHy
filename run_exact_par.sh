#!/bin/bash

# Compile the program
g++ -O3 -std=c++11 -lgomp -fopenmp main_exact_par.cpp -o exact_par

# Find all .txt files in the current directory
for dataset in *.txt; do
    # Extract the base name without the .txt extension for the CSV file name
    dataset_name=$(basename "$dataset" .txt)
    
    # Initialize the CSV file for this dataset
    echo "Threads,Time (seconds)" > "${dataset_name}_results.csv"
    
    # Loop through the thread counts
    for i in 1 2 4 8 16 32 64; do
        # Execute the program and capture the output
        time=$(./exact_par $i "$dataset" | grep "Motif counting" | awk '{print $3}')
        
        # Write the thread count and time to the dataset-specific CSV file
        echo "$i,$time" >> "${dataset_name}_results.csv"
    done
    
    # Plot the results for this dataset
    python plot.py "${dataset_name}_results.csv"
done

# Clean up by removing the executable
rm exact_par
