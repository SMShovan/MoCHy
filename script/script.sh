#!/bin/bash

# Generate Dataset
FILE='input.txt'
VECTOR_LENGTH=10
SET_SIZE_MIN=3
SET_SIZE_MAX=7
VALUE_MIN=1
VALUE_MAX=9

g++ generateRandomHyperedgeDataset.cpp -o generateRandomHyperedgeDataset

./generateRandomHyperedgeDataset "$FILE" "$VECTOR_LENGTH" "$SET_SIZE_MIN" "$SET_SIZE_MAX" "$VALUE_MIN" "$VALUE_MAX"
echo "Dataset generated successfully"




# Main file
g++ main.cpp -o main && ./main "$FILE"
echo "Main file executed successfully"