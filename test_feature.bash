#!/bin/bash

# This script is used to execute assembly examples and check for their return value through gdb

function cleanup {
    echo "Cleaning up"
    kill $(jobs -p)
}

trap cleanup EXIT

if [[ -x "$(command -v gdb-multiarch)" ]]; then
    gdb=gdb-multiarch
elif [[ -x "$(command -v arm-none-eabi-gdb)" ]]; then
    gdb=arm-none-eabi-gdb
else
    echo "No gdb executable found, please install gdb-multiarch or arm-none-eabi-gdb"
    exit 1
fi

# Check for simulator executable and start it
if [ ! -x ./arm_simulator ]; then
    echo "arm_simulator executable not found or not executable, please compile it first with 'make'"
    exit 1
fi

./arm_simulator --gdb-port 28546 &
if [ $? -ne 0 ]; then
    echo "Failed to start simulator"
    exit 1
fi

tests=()
tests+=("Examples/test_ldr_str")

for test in "${tests[@]}"; do
    echo "Testing $test"
    $gdb --quiet -x gdb_test --args $test $port
    if [ $? -ne 0 ]; then
        echo "Test $test failed"
        kill $(jobs -p)
        exit 1
    else
        echo "Test $test passed"
    fi
done
