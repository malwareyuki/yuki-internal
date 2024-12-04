.code


nt_read_virtual_memory PROC
    mov r10, rcx
    mov eax, 3Fh
    syscall
    ret
nt_read_virtual_memory ENDP

nt_write_virtual_memory PROC
    mov r10, rcx
    mov eax, 3Ah
    syscall
    ret
nt_write_virtual_memory ENDP



end