# ARM-SIM

# ARMv5 Instruction Set Simulator

## Description

This project aims to develop a simulator for a subset of the ARMv5 instruction set. The simulator is designed to execute machine code written for the ARMv5 instruction set on a host machine with a different instruction set. While the primary target host architecture is Intel x86 (32-bit) or x86-64 (64-bit), the simulator code is written portably and can be compiled and executed on other architectures.

The project relies on the `gdb` debugger in client-server mode, specifically `arm-none-eabi-gdb`, to extract sections from ELF executable files and load them into the simulator's memory. It also allows for step-by-step execution and interaction with the simulator's memory and registers.

The simulator (server) and `arm-none-eabi-gdb` (client) interact via a TCP/IP network communication channel and can thus be launched on the same machine or on different machines.

## Installation

Detailed installation instructions will be added once the development process reaches a stable point.

## Usage

Instructions on how to use the simulator will be provided, including how to interact with the simulator via `arm-none-eabi-gdb`, execute instructions step-by-step, and inspect the state of the simulated machine.

## Testing

Once the project is in a more mature state, instructions for running tests will be provided here.

## Acknowledgements

This project uses resources available on [ARM's official website](https://www.arm.com) for the ARMv5 reference manual and `arm-none-eabi-gdb` for debugging capabilities.

