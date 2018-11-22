section .textcmpr

global call

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
