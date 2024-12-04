.code

?vmx_vmcall@hv@@YA_KAEAUhypercall_input@1@@Z proc
  ; move input into registers
  mov rax, [rcx]       ; code
  mov rdx, [rcx + 10h] ; args[1]
  mov r8,  [rcx + 18h] ; args[2]
  mov r9,  [rcx + 20h] ; args[3]
  mov r10, [rcx + 28h] ; args[4]
  mov r11, [rcx + 30h] ; args[5]
  mov rcx, [rcx + 08h] ; args[0]

  vmcall

  ret
?vmx_vmcall@hv@@YA_KAEAUhypercall_input@1@@Z endp

_spoofer_stub proc

pop r11
add rsp, 8
mov rax, [rsp + 24]

mov r10, [rax]
mov [rsp], r10

mov r10, [rax + 8]
mov [rax + 8], r11

mov [rax + 16], rbx
lea rbx, fixup
mov [rax], rbx
mov rbx, rax

jmp r10

fixup:
sub rsp, 16
mov rcx, rbx
mov rbx, [rcx + 16]
jmp QWORD PTR [rcx + 8]

_spoofer_stub endp
end