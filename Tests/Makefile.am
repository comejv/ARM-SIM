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
noinst_PROGRAMS=test_ldr_str test_data_processing test_ldm_stm

all-am: $(PROGS)
endif
test_data_processing_SOURCES=test_data_processing.s
test_ldr_str_SOURCES=test_ldr_str.s
test_ldm_stm_SOURCES=test_ldm_stm.s

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)