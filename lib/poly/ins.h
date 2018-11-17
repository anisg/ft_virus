#ifndef INS_H
# define IND_H

/*
 * 90 	nop
 */
# define INS_NOP "\x90"

/*
 * 55 	pushq	%rbp
 * 48 89 e5 	movq	%rsp, %rbp
 * c9 	leave
 * c3 	retq
 */
# define INS_PUSH_RBP "\x55"
# define INS_MOV_RSP_RBP "\x48\x89\xe5"
# define INS_LEAVEQ "\xc9"
# define INS_RETQ "\xc3"

/*
 * 48 89 ce 	movq	%rcx, %rsi
 * 48 ff c7 	incq	%rdi
 */
# define INS_MOV_RCX_RSI "\x48\x89\xf1"
# define INS_INC_RDI "\x48\xff\xc7"

/*
 * cc 	int3
 */
# define INS_INT3 "\xcc"

/*
 * fe 07 	incb	(%rdi)
 * fe 0f 	decb	(%rdi)
 */
# define INS_INCB_RDI "\xfe\x07"
# define INS_DECB_RDI "\xfe\x0f"

/*
 * d0 07 	rolb	(%rdi)
 * d0 0f 	rorb	(%rdi)
 */
# define INS_RORB_1 "\xd0\x0f"
# define INS_ROLB_1 "\xd0\x07"

/*
 * c0 07 ab 	rolb	$171, (%rdi)
 * c0 0f ab 	rorb	$171, (%rdi)
 */
# define INS_RORB_I "\xc0\x0f"
# define INS_ROLB_I "\xc0\x07"

/*
 * 80 37 0c 	xorb	$12, (%rdi)
 */
# define INS_XORB_I "\x34"

/*
 * 32 0f 	xorb	(%rdi), %cl
 * 32 2f 	xorb	(%rdi), %ch
 */
# define INS_XORB_CL "\x30\x0f"
# define INS_XORB_CH "\x30\x2f"

/*
 * 4c 8b 02 	movq	(%rdx), %r8
 * 4c 8b 4a 08 	movq	8(%rdx), %r9
 */
# define INS_K1_R8 "\x4c\x8b\x02"
# define INS_K2_R9 "\x4c\x8b\x4a\x08"

/*
 * 44 30 07 	xorb	%r8b, (%rdi)
 * 44 30 0f 	xorb	%r9b, (%rdi)
 */
# define INS_XORB_R8 "\x44\x30\x07"
# define INS_XORB_R9 "\x44\x30\x0f"

/*
 * 49 c1 c8 0b 	rorq	$11, %r8
 * 49 c1 c9 0b 	rorq	$11, %r9
 */
# define INS_ROR_R8 "\x49\xc1\xc8\x0b"
# define INS_ROR_R9 "\x49\xc1\xc9\x0b"

#endif
