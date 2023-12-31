#!/bin/bash

# This script is used to execute assembly examples and check for their return value through gdb
if [[ -x "$(command -v gdb-multiarch)" ]]; then
    gdb=gdb-multiarch
elif [[ -x "$(command -v arm-none-eabi-gdb)" ]]; then
    gdb=arm-none-eabi-gdb
else
    echo -e "\x1b[1;31mError: gdb-multiarch or arm-none-eabi-gdb not found\x1b[0m"
    exit 1
fi

# Check for simulator executable and start it
if [[ ! -x ./arm_simulator ]]; then
    echo -e "x1b[1;31mError: arm_simulator not found\x1b[0m"
    exit 1
fi

tests=()
tests+=("Tests/test_ldr_str")
tests+=("Tests/test_data_processing")
tests+=("Tests/test_ldm_stm")

for test in "${tests[@]}"; do
    echo -e "\x1b[34mInfo : Running test $test\x1b[0m"

    ./arm_simulator --gdb-port 28546 &
    if [[ $? -ne 0 ]]; then
        echo -e "\x1b[1;31mError: arm_simulator failed to start\x1b[0m"
        exit 1
    fi

    $gdb --batch-silent -nh -x gdb_test --args $test $port
    ret=$?
    if [[ ret -ne 0 ]]; then
        echo -e "\x1b[1;31mTest $test failed with output $ret\x1b[0m"
        exit=$((exit + 1))
    else
        echo -e "\x1b[1;32mTest $test passed\x1b[0m"
    fi
    echo "---"
done

echo "---"
if [[ $exit -eq 0 ]]; then
    echo -e "\x1b[1;32mAll tests passed\x1b[0m"
else
    echo -e "\x1b[1;31m$exit tests failed\x1b[0m"
fi

exit $exit
