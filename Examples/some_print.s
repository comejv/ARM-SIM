	.cpu arm7tdmi
	.arch armv4t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"some_print.c"
	.text
	.align	2
	.global	fibo
	.syntax unified
	.arm
	.type	fibo, %function
fibo:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	str	lr, [sp, #-4]!
	sub	sp, sp, #20
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L4
	ldr	r3, [sp, #8]
	and	r3, r3, #255
	add	r3, r3, #48
	and	r3, r3, #255
	mov	r0, r3
	bl	my_putchar
	mov	r0, #10
	bl	my_putchar
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	add	r1, r2, r3
	ldr	r3, [sp, #4]
	sub	r3, r3, #1
	mov	r2, r3
	ldr	r0, [sp, #8]
	bl	fibo
	b	.L1
.L4:
	nop
.L1:
	add	sp, sp, #20
	@ sp needed
	ldr	lr, [sp], #4
	bx	lr
	.size	fibo, .-fibo
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r2, #5
	mov	r1, #1
	mov	r0, #1
	bl	fibo
	mov	r3, #0
	mov	r0, r3
	pop	{r4, lr}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (GNU Arm Embedded Toolchain 10.3-2021.10) 10.3.1 20210824 (release)"
