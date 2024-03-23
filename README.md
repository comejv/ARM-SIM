# ARMv5 Instruction Set Simulator

## Description

This project aims to develop a simulator for a subset of the ARMv5 instruction set. The simulator is designed to execute machine code written for the ARMv5 instruction set on a host machine with a different instruction set. While the primary target host architecture is Intel x86 (32-bit) or x86-64 (64-bit), the simulator code is written portably and can be compiled and executed on other architectures.

The project relies on the `gdb` debugger in client-server mode, specifically `gdb-multiarch` or `arm-none-eabi-gdb`, to extract sections from ELF executable files and load them into the simulator's memory. It also allows for step-by-step execution and interaction with the simulator's memory and registers.

The simulator (server) and `gdb` (client) interact via a TCP/IP network communication channel and can thus be launched on the same machine or on different machines.

## Installation

To install the project first download the repository, then in the project directory run `autoreconf && ./configure CFLAGS='-Wall -Werror -g -O0'` and then you can `make`. You should obtain an executable file `./arm_simulator`.

## Usage

The main program, arm_simulator acts as a gdb server, so a typical use session
would look like:
`./arm_simulator`
this starts the simulator which waits for two kinds of incoming connections:
- a connection from a gdb client
- a connection from the external command used to send irqs to the simulator
the simulator prints on the screen the ports numbers on which it listens for
these connections.

To use the simulator, you must connect to it a gdb client:
```gdb
arm-none-eabi-gdb
file Tests/foo (for instance)
target remote localhost:<port number given by the simulator>
set endian big
load
step
break 8
cont
```
... and so on

Debugging messages and traces outputed by the simulator can be chosen at
compile-time using compilation flags. Just comment the undesired flags settings
in the first lines of Makefile.am, then `make clean && make`.

## Testing

Tests are available for these features :
- [X] Simulated memory : `./memory_test`
- [X] Registers : `./registers_test`
- [X] Memory access : `bash test_features.bash`
- [X] Branch and link : `bash test_features.bash`
- [X] Arithmetic instructions : `bash test_features.bash`
- [X] Inter register instructions : `bash test_features.bash`
- [ ] Coprocessor instructions (not implemented)

## Showcases

A few examples have been created and tailored to showcase the capabilities of our program. They've been written in C and will be compiled by a regular compiler.
To run the examples you first have to start the simulator with `./arm_simulator`.
Then to run the showcase you want, you may use the following commands : 
```gdb
arm-none-eabi-gdb
file Showcase/<program_name>
target remote localhost:<port number given by the simulator>
set endian big
c
```
- chenillion.c and chenillion_rainbow.c : show flowing text on a single line.
- tictactoe.c : play a 2 players tic tac toe on the simulator.
- clock.c : self updating text using software interrupts.
- fibonacci.c : test proper handling of the call stack.

Inputs have to be given to the main simulator program.

## Acknowledgements

This project uses resources available on [ARM's official website](https://www.arm.com) for the ARMv5 reference manual and `arm-none-eabi-gdb` to read binary files.
