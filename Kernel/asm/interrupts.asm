
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL _setUser

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL _syscallHandler

GLOBAL regs
GLOBAL regsSaved


EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN getStackBase



SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	push rax
    saveRegs
    pop rax
    mov [regs], rax

    mov rax, userland_direc 
    mov [rsp], rax          ; jump a userland

    mov rax, 0x8
    mov [rsp + 8], rax      ; CS de userland

    mov rax, 0x202
    mov [rsp + 8*2], rax    ; RFLAGS

    call getStackBase       
    mov [rsp + 8*3], rax    ; sp ahora esta en la base 

    mov rax, 0x0
    mov [rsp + 8*4], rax    ; SS de userland

	pushState

	mov rdi, %1              ; pasaje de parametro
	; mov rsi, rsp ; testeo..
	call exceptionDispatcher

    popState

	iretq
%endmacro


%macro saveRegs 0
	mov [regs+8], rbx
	mov [regs+8*2], rcx
	mov [regs+8*3], rdx
	mov [regs+8*4], rsi
	mov [regs+8*5], rdi
	mov [regs+8*6], rbp
	mov [regs+8*7], rsp
	mov [regs+8*8], r8
	mov [regs+8*9], r9 
	mov [regs+8*10], r10
	mov [regs+8*11], r11
	mov [regs+8*12], r12
	mov [regs+8*13], r13
	mov [regs+8*14], r14
	mov [regs+8*15], r15

	mov rax, [rsp + 8]			; RIP
	mov [regs+8*16], rax			

	mov rax, [rsp+8*3]		; RFLAGS
	mov [regs+8*17], rax
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret


_setUser:

	sub rsp, 32             

    mov rax, userland_direc 
    mov [rsp], rax          ; preparo el salto a userland 

    mov rax, 0x8
    mov [rsp + 8], rax      ; CS de userland

    mov rax, 0x202
    mov [rsp + 8*2], rax    ; RFLAGS

    call getStackBase       
    mov [rsp + 8*3], rax    ; sp ahora esta en la base 

    mov rax, 0x0
    mov [rsp + 8*4], rax    ; SS de userland
	
	iretq

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	push rax  
	mov rax, 0 ; clean
 
	in al, 60h 

	cmp al, 0x1D  ; ver bien para que sea ctrl 
	jne .irqHandler
	saveRegs
	mov [regsSaved], byte 1
	mov rax, [rsp]
	mov [regs], rax

.irqHandler:
	pop rax
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

; syscall number provided in rax
_syscallHandler:

	mov r9, rax 
	call syscallDispatcher

	iretq

haltcpu:
	cli
	hlt
	ret


SECTION .rodata

userland_direc equ 0x400000



section .data
regsSaved db 0

SECTION .bss
	aux resq 1
	regs resq 18