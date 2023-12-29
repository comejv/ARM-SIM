#!/bin/sh
gdb=arm-none-eabi-gdb



rm -r Tests/trace
mkdir Tests/trace
for file in Tests/test*.s
do
  base=`expr "$file" : 'test/\(.*\)\.s'`
  ./arm_simulator --gdb-port 58000 --trace-registers --trace-memory --trace-state SVC \
                  >Tests/trace/trace_$base &
 $gdb -ex "file Tests/$base" -x gdb_commands --batch
done
zip -9 -j student/traces.zip Tests/trace/*
 