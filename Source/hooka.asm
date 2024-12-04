.code
__query_virtual_memory proc
mov     r10, rcx
mov     eax, 23h
syscall 
ret
__query_virtual_memory endp
end