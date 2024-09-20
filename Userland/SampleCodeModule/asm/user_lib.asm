GLOBAL _invalid_opcode_exception


section .text 

_invalid_opcode_exception:
    mov cr6, rax
    ret


