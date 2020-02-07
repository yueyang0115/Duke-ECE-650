#!/bin/bash
echo "Making"
make clean
make
for i in {1..200}
do
echo "Testing $i thread_test"
./thread_test
echo "Testing $i thread_test_malloc_free"
./thread_test_malloc_free
echo "Testing $i thread_test_malloc_free_change_thread"
./thread_test_malloc_free_change_thread 
echo "Testing $i thread_test_measurement"
./thread_test_measurement
done
echo "Cleaning"
make clean
