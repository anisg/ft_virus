section .textearly

global call
global restore_rt

call:
	push   rbp
	mov    rbp, rsp
	mov    rax, rcx
	mov    r10, r8
	mov    r8, r9
	mov    r9, [rbp+0x10]
	syscall
	leave
	ret

restore_rt:
	push rbp
	mov rbp, rsp
	leave
	mov rax, 15
	syscall
	leave
	ret
