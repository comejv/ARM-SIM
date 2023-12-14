#!/bin/sh
gdb=arm-none-eabi-gdb

if [ ! -x ./arm_simulator -o ! -d student ]
then
	echo missing simulator or student version
	exit 1
fi

rm -r Examples/trace
mkdir Examples/trace
for file in Examples/example*.s
do
  base=`expr "$file" : 'Examples/\(.*\)\.s'`
  ./arm_simulator --gdb-port 58000 --trace-registers --trace-memory --trace-state SVC \
                  >Examples/trace/trace_$base &
 $gdb -ex "file Examples/$base" -x gdb_commands --batch
done
zip -9 -j student/traces.zip Examples/trace/*
