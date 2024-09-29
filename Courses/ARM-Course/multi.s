.section .text
.global _start

_start:
	ldr r0, =mydata
	ldm r0, {r1,r2,r3,r4,r5,r6,r7,r8}

	stm r0, {r1,r2,r3,r4,r5,r6,r7,r8}

.section .data
mydata:
	.word 1,2,3,4,5,6,7,8

