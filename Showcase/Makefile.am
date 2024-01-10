CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
# Our simulator :
# - is big-endian
# - does not initialize properly fp (so we omit it), see arm_core.c
# - has a non standard main (no OS so no return value), we disable the warning
AM_CFLAGS=-mbig-endian -fomit-frame-pointer -Wno-main
# Still big-endian for ARM assembly language sources
AM_CCASFLAGS=-mbig-endian
# We don't have any OS, so no stdlib as well
# The linker script maps loadable segments into memory, the overall size must match the simulator memory
# The entry point will always be main
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
# Our simulator is big-endian
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=insertion_sort fibonacci clock chenillion chenillion_rainbow tictactoe

all-am: $(PROGS)
endif
clock_SOURCES=clock.c my_syscalls.s my_syscalls.h
fibonacci_SOURCES=fibonacci.c my_syscalls.s my_syscalls.h
insertion_sort_SOURCES=insertion_sort.c my_syscalls.s my_syscalls.h
chenillion_SOURCES=chenillion.c my_syscalls.s my_syscalls.h my_utils.c my_utils.h
chenillion_rainbow_SOURCES = chenillion_rainbow.c my_syscalls.s my_syscalls.h my_utils.c my_utils.h
tictactoe_SOURCES = tictactoe.c my_syscalls.s my_syscalls.h

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
