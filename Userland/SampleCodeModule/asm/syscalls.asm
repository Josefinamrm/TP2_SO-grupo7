GLOBAL _write
GLOBAL _read
GLOBAL _change_draw_size
GLOBAL _print_array_of_draw_size
GLOBAL _getRegisters
GLOBAL _sleep
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

; processes:
GLOBAL _get_pid
GLOBAL _create_process
GLOBAL _nice
GLOBAL _kill
GLOBAL _block
GLOBAL _unblock
GLOBAL _yield
GLOBAL _wait_processes
GLOBAL _get_processes_info
GLOBAL _exit

; memory:

GLOBAL _malloc
GLOBAL _realloc
GLOBAL _free
GLOBAL _unused_space
GLOBAL _occupied_space
GLOBAL _total_space

;semaphores

GLOBAL _sem_open
GLOBAL _sem_close
GLOBAL _sem_post
GLOBAL _sem_wait

; ipc

GLOBAL _open_pipe
GLOBAL _close_pipe
GLOBAL _write_pipe
GLOBAL _read_pipe
GLOBAL _open_fd
GLOBAL _close_fd
GLOBAL _close_all_fds
GLOBAL _write_to_fd
GLOBAL _read_from_fd

section .text

_read: 
    push rbp 
    mov rbp, rsp 

    mov rax, 0
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

_write:
    push rbp
    mov rbp, rsp 

    mov rax, 1
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

_getRegisters:
    push rbp
    mov rbp, rsp 

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_sleep:
    push rbp
    mov rbp, rsp 

    mov rax, 9
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



_flush_buffer:
    push rbp 
    mov rbp, rsp 

    mov rax, 15
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_pid:
    push rbp 
    mov rbp, rsp 

    mov rax, 16
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_create_process:
    push rbp 
    mov rbp, rsp 

    mov rax, 17
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_nice:
    push rbp 
    mov rbp, rsp 

    mov rax, 18
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_kill:
    push rbp 
    mov rbp, rsp 

    mov rax, 19
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_block:
    push rbp 
    mov rbp, rsp 

    mov rax, 20
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_unblock:
    push rbp 
    mov rbp, rsp 

    mov rax, 21
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_yield:
    push rbp 
    mov rbp, rsp 

    mov rax, 22
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_wait_processes:
    push rbp 
    mov rbp, rsp 

    mov rax, 23
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_processes_info:
    push rbp 
    mov rbp, rsp 

    mov rax, 24
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_exit:
    push rbp 
    mov rbp, rsp 

    mov rax, 25
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_malloc:
    push rbp 
    mov rbp, rsp 

    mov rax, 26
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_realloc:
    push rbp 
    mov rbp, rsp 

    mov rax, 27
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_free:
    push rbp 
    mov rbp, rsp 

    mov rax, 28
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_unused_space:
    push rbp 
    mov rbp, rsp 

    mov rax, 29
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_occupied_space:
    push rbp 
    mov rbp, rsp 

    mov rax, 30
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_total_space:
    push rbp 
    mov rbp, rsp 

    mov rax, 31
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_sem_open:
    push rbp 
    mov rbp, rsp 

    mov rax, 32
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_sem_close:
    push rbp 
    mov rbp, rsp 

    mov rax, 33
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_sem_post:
    push rbp 
    mov rbp, rsp 

    mov rax, 34
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_sem_wait:
    push rbp 
    mov rbp, rsp 

    mov rax, 35
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_open_pipe:
    push rbp 
    mov rbp, rsp 

    mov rax, 36
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_close_pipe:
    push rbp 
    mov rbp, rsp 

    mov rax, 37
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_write_pipe:
    push rbp 
    mov rbp, rsp 

    mov rax, 38
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_read_pipe: 
    push rbp 
    mov rbp, rsp 

    mov rax, 39
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_open_fd:
    push rbp 
    mov rbp, rsp 

    mov rax, 40
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_close_fd:
    push rbp 
    mov rbp, rsp 

    mov rax, 41
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_close_all_fds:
    push rbp 
    mov rbp, rsp 

    mov rax, 42
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_write_to_fd:
    push rbp 
    mov rbp, rsp 

    mov rax, 43
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_read_from_fd:
    push rbp 
    mov rbp, rsp 

    mov rax, 44
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret