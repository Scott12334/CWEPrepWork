.section .text
.global _start

_start:
	ldr r0, =#1
	ldr r1, =#2
	bl foo
	mov r0,r0

foo:
	add r0, r0, r1
	mov pc, lr
