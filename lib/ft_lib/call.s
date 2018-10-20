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
	mov    rax, 0xf
	syscall
