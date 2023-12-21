#!/bin/sh
gdb=arm-none-eabi-gdb



rm -r Examples/trace
mkdir Examples/trace
for file in Examples/test*.s
do
  base=`expr "$file" : 'test/\(.*\)\.s'`
  ./arm_simulator --gdb-port 58000 --trace-registers --trace-memory --trace-state SVC \
                  >Examples/trace/trace_$base &
 $gdb -ex "file Examples/$base" -x gdb_commands --batch
done
zip -9 -j student/traces.zip Examples/trace/*
 