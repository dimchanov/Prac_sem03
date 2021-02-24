.intel_syntax noprefix
.section .data
n: .long  0
mas: .long 0
.section .rodata
format: .asciz "%d "
.EXTERN printf
.EXTERN scanf
.EXTERN malloc
.section .text
.global main 
main:
mov ebp, esp
sub esp, 8
mov ebp, esp
sub esp, 8
mov eax, 0
mov esi, n
mov edi, format
call scanf
mov eax, 1
mov esi, [n]
mov edi, format
call printf
xor eax, eax
ret
