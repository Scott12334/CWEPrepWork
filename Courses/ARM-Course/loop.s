.section .text
.global _start

_start:
	ldr r0, =#0

loop:
	cmp r0, #10
	bge end

	add r0, #1
	b loop

end:
	b _start
