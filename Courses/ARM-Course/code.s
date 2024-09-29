.section .text
.global _start

_start:
	ldr r0, =#10
	b other
	ldr r0, =#13371337

other:
	cmp r0, #10
	moveq r1, #0xfefe
