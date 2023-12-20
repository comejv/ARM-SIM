#!/bin/bash

# This script is used to execute assembly examples and check for their return value through gdb



# Check for simulator executable and start it
if [ ! -x ./arm_simulator ]; then
    echo "arm_simulator executable not found or not executable, please compile it first with 'make'"
    exit 1
fi

./arm_simulator --gdb-port 69420 &

tests=
tests+=("example1.s")

for test in "${tests[@]}"; do
    echo "Testing $test"
    gdb --batch -x gdb_test --gdb-port 69420 --args $test
    if [ $? -ne 0 ]; then
        echo "Test $test failed"
        exit 1
    fi
done
