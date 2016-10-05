	.text
	.extern int_handler

handler_common:
	pushq %rsp
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rdi
	pushq %rsi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, %rdi
	cld
	call int_handler
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	popq %rsi
	popq %rdi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %rsp
	addq $16, %rsp 
	iretq

.global table
table:
			.quad entry_0
			.quad entry_1
			.quad entry_2
			.quad entry_3
			.quad entry_4
			.quad entry_5
			.quad entry_6
			.quad entry_7
			.quad entry_8
			.quad entry_9
			.quad entry_10
			.quad entry_11
			.quad entry_12
			.quad entry_13
			.quad entry_14
			.quad entry_15
			.quad entry_16
			.quad entry_17
			.quad entry_18
			.quad entry_19
			.quad entry_20
			.quad entry_21
			.quad entry_22
			.quad entry_23
			.quad entry_24
			.quad entry_25
			.quad entry_26
			.quad entry_27
			.quad entry_28
			.quad entry_29
			.quad entry_30
			.quad entry_31
			.quad entry_32

entry_0:
	pushq $0
	pushq $0
	jmp handler_common

entry_1:
	pushq $0
	pushq $1
	jmp handler_common

entry_2:
	pushq $0
	pushq $2
	jmp handler_common

entry_3:
	pushq $0
	pushq $3
	jmp handler_common

entry_4:
	pushq $0
	pushq $4
	jmp handler_common

entry_5:
	pushq $0
	pushq $5
	jmp handler_common

entry_6:
	pushq $0
	pushq $6
	jmp handler_common

entry_7:
	pushq $0
	pushq $7
	jmp handler_common

entry_8:
	pushq $8
	jmp handler_common

entry_9:
	pushq $0
	pushq $9
	jmp handler_common

entry_10:
	pushq $0
	pushq $10
	jmp handler_common

entry_11:
	pushq $11
	jmp handler_common

entry_12:
	pushq $12
	jmp handler_common

entry_13:
	pushq $13
	jmp handler_common

entry_14:
	pushq $14
	jmp handler_common

entry_15:
	pushq $0
	pushq $15
	jmp handler_common

entry_16:
	pushq $0
	pushq $16
	jmp handler_common

entry_17:
	pushq $17
	jmp handler_common

entry_18:
	pushq $0
	pushq $18
	jmp handler_common

entry_19:
	pushq $0
	pushq $19
	jmp handler_common

entry_20:
	pushq $0
	pushq $20
	jmp handler_common

entry_21:
	pushq $0
	pushq $21
	jmp handler_common

entry_22:
	pushq $0
	pushq $22
	jmp handler_common

entry_23:
	pushq $0
	pushq $23
	jmp handler_common

entry_24:
	pushq $0
	pushq $24
	jmp handler_common

entry_25:
	pushq $0
	pushq $25
	jmp handler_common

entry_26:
	pushq $0
	pushq $26
	jmp handler_common

entry_27:
	pushq $0
	pushq $27
	jmp handler_common

entry_28:
	pushq $0
	pushq $28
	jmp handler_common

entry_29:
	pushq $0
	pushq $29
	jmp handler_common

entry_30:
	pushq $0
	pushq $30
	jmp handler_common

entry_31:
	pushq $0
	pushq $31
	jmp handler_common

entry_32:
	pushq $0
	pushq $32
	jmp handler_common