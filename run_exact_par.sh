g++ -O3 -std=c++11 -lgomp -fopenmp main_exact_par.cpp -o exact_par;
# Create or clear the file
echo "Threads,Time (seconds)" > motif_counting_results.csv

# Loop through the thread counts
for i in 1 2 4 8 16 32 64; do
    # Execute the program and capture the output
    time=$(./exact_par $i | grep "Motif counting" | awk '{print $3}')
    
    # Write the thread count and time to the CSV file
    echo "$i,$time" >> motif_counting_results.csv
done
rm exact_par;
