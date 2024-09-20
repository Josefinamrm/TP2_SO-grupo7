GLOBAL _write
GLOBAL _read
GLOBAL _change_draw_size
GLOBAL _print_array_of_draw_size
GLOBAL _getRegisters
GLOBAL _wait
GLOBAL _write_color
GLOBAL _clear_screen
GLOBAL _change_font_size
GLOBAL _get_font_size
GLOBAL _beep
GLOBAL _print_square
GLOBAL _print_rect
GLOBAL _get_time
GLOBAL _draw_array
GLOBAL _flush_buffer
section .text

_write:
    push rbp
    mov rbp, rsp 

    mov rax, 1
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_getRegisters:
    push rbp
    mov rbp, rsp 

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_wait:
    push rbp
    mov rbp, rsp 

    mov rax, 9
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_read: 
    push rbp 
    mov rbp, rsp 

    mov rax, 0
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

_write_color: 
    push rbp 
    mov rbp, rsp 

    mov rax, 6
    int 80h

    mov rsp, rbp 
    pop rbp 
    ret

_clear_screen: 
    push rbp 
    mov rbp, rsp 

    mov rax, 7
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret 

_change_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 10
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 11 
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret
_beep: 
    push rbp
    mov rbp, rsp 

    mov rax, 12 
    int 80h

    mov rsp, rbp 
    pop rbp
    ret

_print_square: 
    push rbp 
    mov rbp, rsp 

    mov rax, 13
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

_print_rect:
    push rbp 
    mov rbp, rsp 

    mov rax, 14
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_time: 
    push rbp 
    mov rbp, rsp 

    mov rax, 2
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_change_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 3
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_print_array_of_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 4
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_draw_array: 
    push rbp 
    mov rbp, rsp 

    mov rax, 5
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_flush_buffer:
    push rbp 
    mov rbp, rsp 

    mov rax, 15
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret