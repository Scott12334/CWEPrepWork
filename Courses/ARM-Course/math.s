.global _start
.section .text

_start:
	ldr r0, =#0xffffffff
	ldr r1, =#0xffffffff

	adds r0, r0, r1

loop:
	b loop
