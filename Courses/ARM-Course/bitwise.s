.global _start
.section .text

_start:
	ldr r0, =#0x12
	ldr r1, =#0x34

	eor r0, r0, r1
	eor r1, r0, r1
	eor r0, r0, r1

loop:
	b loop
