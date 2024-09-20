GLOBAL cpuVendor
GLOBAL getScanCode 
GLOBAL getTime
GLOBAL getHours
GLOBAL getMinutes
GLOBAL getSeconds
GLOBAL inb 
GLOBAL outb 

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


; Función para obtener la hora del RTC
; recibe en rdi un número del 0 al 9 para seleccionar la información del RTC
getTime:
    push rbp
    mov rbp, rsp
	push  rdi
	pushf
    
    .wait:
        cli 
        mov al, 0x0A  
        out 70h, al
        in al, 71h
        sti
        and al, 0x8

        cmp al, 1
        je .wait

        cli
        mov rax, rdi
        out 70h, al
        in al, 71h           
        sti

	popf 
	pop rdi
    mov rsp,rbp
    pop rbp
    ret


; Función para obtener las horas del RTC
getHours:
	push rbp
	mov rbp, rsp 
	push rdi
	pushf

	mov rdi, 4 	; 4 es el número correspondiente a las horas en el RTC
	call getTime

	popf
	pop rdi 
	mov rsp, rbp
	pop rbp
	ret



; Función para obtener los minutos del RTC
getMinutes:
	push rbp
	mov rbp, rsp 
	push rdi
	pushf

	mov rdi, 2        ; 2 es el número correspondiente a los minutos en el RTC
	call getTime

	popf
	pop rdi 
	mov rsp, rbp
	pop rbp
	ret


; Función para obtener los segundos del RTC
getSeconds:
	push rbp
	mov rbp, rsp 
	push rdi
	pushf

	mov rdi, 0        ; 0 es el número correspondiente a los segundos en el RTC
	call getTime

	popf
	pop rdi 
	mov rsp, rbp
	pop rbp
	ret


; Función para obtener el código de escaneo del teclado
getScanCode: 
	push rbp 
	mov rbp, rsp 
	mov rax, 0 

	in al, 60h       ; Leer del puerto 60h (puerto del teclado)
	
	mov rsp, rbp 
	pop rbp 
	ret


; Función para leer un byte del puerto especificado en rdi
inb:
	push rbp
	mov rbp, rsp 
	pushf

	mov rax, 0
	mov rdx, rdi
    in al, dx        ; Leer un byte del puerto dx en al

	popf
	mov rsp, rbp
	pop rbp
	ret

; Función para escribir un byte al puerto especificado en rdi, con el valor en rsi

outb:
	push rbp
	mov rbp, rsp 
	pushf

	mov rax, rsi 
	mov rdx, rdi 
	out dx, al       ; Escribir el byte de al al puerto dx

	popf
	mov rsp, rbp
	pop rbp
	ret