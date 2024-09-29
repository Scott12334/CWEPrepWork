.section .data
prompt:		.asciz "$ "
buffer:		.space 256

.section .text
.global _start

_start:
	bl main

main:
	bl display
	bl read
	bl execute
	b main

read:
	push {r4-r11, lr}
	mov r0, #0
	ldr r1, =buffer
	mov r2, #256
	mov r7, #3
	svc #0
	bl stripInput
	pop {r4-r11, pc}

stripInput:
	push {r4-r11, lr}
	mov r2, #0
	ldr r1, =buffer
stripLoop:
	ldrb r3, [r1, r2]
	cmp r3, #0xa
	beq _removeNewline
	cmp r3, #0x0
	beq _endLoop
	add r2, r2, #1
	b stripLoop
_removeNewline:
	mov r0, #0
	add r3, r1, r2
	strb r0, [r3]
_endLoop:
	pop {r4-r11, pc}

display:
	push {r4-r11, lr}
	ldr r0, =prompt
	bl printString
	pop {r4-r11, pc}

printString:
	push {r4-r11, lr}
	mov r1, r0
	mov r2, #0

_printLoop:
	ldrb r3, [r1,r2]
	cmp r3, #0
	beq _endPrint
	add r2, r2, #1
	b _printLoop

_endPrint:
	mov r2, r2
	mov r7, #4
	mov r0, #1
	svc #0
	pop {r4-r11, pc}

execute:
	push {r4-r11, lr}
	ldr r0, =buffer
	bl parseCommand
	cmp r0, #0
	beq endExecute
	bl forkProc
	cmp r0, #0
	beq childProc
	bl waitForChild
endExecute:
	pop {r4-r11, pc}
	
parseCommand:
	push {lr}
	mov r0, r0
	pop {pc}

forkProc:
	push {r4-r11, lr}
	mov r7, #2
	svc #0
	pop {r4-r11, pc}

childProc:
	push {r4-r11, lr}
	ldr r0, =buffer
	mov r1, #0
	mov r2, #0
	mov r7, #11
	svc #0
	mov r7, #1
	svc #0

waitForChild:
	mov r7, #0x72
	mov r0, #-1
	mov r1, #0
	mov r2, #0
	svc #0
	pop {r4-r11, pc}



