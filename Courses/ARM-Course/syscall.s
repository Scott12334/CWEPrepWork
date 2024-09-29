.section .text
.global _start

_start:
	ldr r7, =#1
	ldr r0, =#0
	svc #0
