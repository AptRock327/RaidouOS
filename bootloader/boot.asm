[org 0x7c00]
mov [DISK_NUM], dl ;Save disk number (just in case)

;Set up stack
mov bp, 0x7c00
mov sp, bp

;Enable A20 line
in al, 0x92
or al, 2
out 0x92, al

;Load first 127 sectors into RAM
mov ax, 0x0000
mov es, ax
mov ah, 2
mov al, 127
mov bx, 0x8000
mov dl, [DISK_NUM]
mov ch, 0
mov dh, 0
mov cl, 2

int 0x13

;Load more sectors (TODO: automation, this is boilerplate)
mov bx, 0

mov ax, 0x17E0
mov es, ax
mov ah, 2
mov al, 127
mov dh, 2
mov cl, 3

int 0x13

mov ax, 0x27C0
mov es, ax
mov ah, 2
mov al, 127
mov dh, 4
mov cl, 4

int 0x13

mov ax, 0x37A0
mov es, ax
mov ah, 2
mov al, 127
mov dh, 6
mov cl, 5

int 0x13

mov ax, 0x4780
mov es, ax
mov ah, 2
mov al, 127
mov dh, 8
mov cl, 6

int 0x13

mov ax, 0x5760
mov es, ax
mov ah, 2
mov al, 127
mov dh, 10
mov cl, 7

int 0x13

mov ax, 0x6740
mov es, ax
mov ah, 2
mov al, 127
mov dh, 12
mov cl, 8

int 0x13

mov ax, 0x7720
mov es, ax
mov ah, 2
mov al, 127
mov dh, 14
mov cl, 9

int 0x13

mov ax, 0x8700
mov es, ax
mov ah, 2
mov al, 127
mov ch, 1
mov dh, 0
mov cl, 10

int 0x13

mov ax, 0x96E0
mov es, ax
mov ah, 2
mov al, 127
mov ch, 1
mov dh, 2
mov cl, 11

int 0x13

jmp 0x801A ;Jump to the kernel loader

DISK_NUM:
	db 0

times 510-($-$$) db 0

dw 0xaa55