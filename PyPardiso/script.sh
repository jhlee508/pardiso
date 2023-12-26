#!bin/bash

NUM_THREADS="1 2 4 8 12 16 20 40 60 80"

for i in $NUM_THREADS; do
    echo "Running with $i threads"
    python pardiso_main.py $i
done