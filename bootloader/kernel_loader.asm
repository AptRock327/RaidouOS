[bits 16]
;GDT

dq 0 ;null descriptor

dw 0xFFFF ;limit
dw 0x0000 ;base
db 0x00 ;base
db 10011011b ;access
db 11001111b ;flags + limit
db 0x00 ;base

dw 0xFFFF ;limit
dw 0x0000 ;base
db 0x00 ;base
db 10010011b ;access
db 11001111b ;flags + limit
db 0x00 ;base

dw 23 ;size
dd 0x8000 ;offset

;Load 1280x1024 mode information
mov ax, 0
mov es, ax
mov ax, 0x4F01
mov cx, 0x411B
mov di, ModeAttributes
int 0x10

;Check for compatibility
cmp [XResolution], word 1280
jne graphics_error
cmp [YResolution], word 1024
jne graphics_error

;Set 1280x1024 mode
mov ax, 0x4F02
mov bx, 0x411B
int 0x10

;Disable interrupts, load GDT and get into protected mode
cli
lgdt [0x8018]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp 8:protected

graphics_error:
    mov ah, 0x0E
    mov al, 7
    int 0x10
    mov al, 'N'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'C'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'M'
    int 0x10
    mov al, 'P'
    int 0x10
    mov al, 'A'
    int 0x10
    mov al, 'T'
    int 0x10
    mov al, 'I'
    int 0x10
    mov al, 'B'
    int 0x10
    mov al, 'L'
    int 0x10
    mov al, 'E'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'G'
    int 0x10
    mov al, 'R'
    int 0x10
    mov al, 'A'
    int 0x10
    mov al, 'P'
    int 0x10
    mov al, 'H'
    int 0x10
    mov al, 'I'
    int 0x10
    mov al, 'C'
    int 0x10
    mov al, 'S'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'M'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'D'
    int 0x10
    mov al, 'E'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'F'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'U'
    int 0x10
    mov al, 'N'
    int 0x10
    mov al, 'D'
    int 0x10
    hlt

; Mandatory information for all VBE revisions
ModeAttributes dw 0 ; mode attributes
WinAAttributes db 0 ; window A attributes
WinBAttributes db 0 ; window B attributes
WinGranularity dw 0 ; window granularity
WinSize dw 0 ; window size
WinASegment dw 0 ; window A start segment
WinBSegment dw 0 ; window B start segment
WinFuncPtr dd 0 ; real mode pointer to window function
BytesPerScanLine dw 0 ; bytes per scan line
; Mandatory information for VBE 1.2 and above
XResolution dw 0 ; horizontal resolution in pixels or characters3
YResolution dw 0 ; vertical resolution in pixels or characters
XCharSize db 0 ; character cell width in pixels
YCharSize db 0 ; character cell height in pixels
NumberOfPlanes db 0 ; number of memory planes
BitsPerPixel db 0 ; bits per pixel
NumberOfBanks db 0 ; number of banks
MemoryModel db 0 ; memory model type
BankSize db 0 ; bank size in KB
NumberOfImagePages db 0 ; number of images
db 1 ; reserved for page function
; Direct Color fields (required for direct/6 and YUV/7 memory models)
RedMaskSize db 0 ; size of direct color red mask in bits
RedFieldPosition db 0 ; bit position of lsb of red mask
GreenMaskSize db 0 ; size of direct color green mask in bits
GreenFieldPosition db 0 ; bit position of lsb of green mask
BlueMaskSize db 0 ; size of direct color blue mask in bits
BlueFieldPosition db 0 ; bit position of lsb of blue mask
RsvdMaskSize db 0 ; size of direct color reserved mask in bits
RsvdFieldPosition db 0 ; bit position of lsb of reserved mask
DirectColorModeInfo db 0 ; direct color mode attributes
; Mandatory information for VBE 2.0 and above
PhysBasePtr dd 0 ; physical address for flat memory frame buffer
dd 0 ; Reserved - always set to 0
dw 0 ; Reserved - always set to 0
; Mandatory information for VBE 3.0 and above
LinBytesPerScanLine dw 0 ; bytes per scan line for linear modes
BnkNumberOfImagePages db 0 ; number of images for banked modes
LinNumberOfImagePages db 0 ; number of images for linear modes
LinRedMaskSize db 0 ; size of direct color red mask (linear modes)
LinRedFieldPosition db 0 ; bit position of lsb of red mask (linear modes)
LinGreenMaskSize db 0 ; size of direct color green mask (linear modes)
LinGreenFieldPosition db 0 ; bit position of lsb of green mask (linear modes)
LinBlueMaskSize db 0 ; size of direct color blue mask (linear modes)
LinBlueFieldPosition db 0 ; bit position of lsb of blue mask (linear modes)
LinRsvdMaskSize db 0 ; size of direct color reserved mask (linear modes)
LinRsvdFieldPosition db 0 ; bit position of lsb of reserved mask (linear modes)
MaxPixelClock dd 0 ; maximum pixel clock (in Hz) for graphics mode
times 190 db 0

[bits 32]

error:
    hlt

protected:
    mov ax, 16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;Set up paging (level 4, 1GiB to be mapped)
    mov edi, 0x1000
    mov cr3, edi
    mov dword [edi], 0x2003 ;PML4T[0]->PDPT
    add edi, 0x1000
    mov dword [edi], 0x3003 ;PDPT[0]->PDT
    add edi, 0x1000
    mov dword [edi], 0x4003 ;PDT[0]->PT
    add edi, 0x1000

    ;Identity map 512MiB
    mov ecx, 0
    .map:
        mov eax, 0x200000 ;2MiB page size
        mul ecx
        or eax, 10000011b
        mov [0x3000 + ecx * 8], eax

        inc ecx
        cmp ecx, 256
        jne .map

    ;Map physical addresses of the framebuffer to the rest of virtual addresses
    mov ecx, 256
    mov ebx, 0x200000
    mov eax, [PhysBasePtr]
    div ebx
    mov ebx, eax
    sub ebx, ecx
    .map2:
        mov eax, 0x200000 ;2MiB page size
        push ecx
        add ecx, ebx ;PhysBasePtr/page size - ecx starting value
        mul ecx
        pop ecx
        or eax, 10000011b
        mov [0x3000 + ecx * 8], eax

        inc ecx
        cmp ecx, 512
        jne .map2

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    or eax, 1 << 16
    mov cr0, eax

    ;Modify GDT
    mov [0x800E], byte 10101111b
	mov [0x8014], byte 10101111b

    jmp 8:long_mode

[bits 64]
[extern main]

long_mode:
	call main
	jmp $

[extern isr0_handler]
isr0:
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
  mov rdi, rsp
  call isr0_handler
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
  iretq
  GLOBAL isr0

[extern isr1_handler]
isr1:
  push rax
  push rcx
  push rdx
  push r8
  push r9
  push r10
  push r11
  call isr1_handler
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdx
  pop rcx
  pop rax
  iretq
  GLOBAL isr1

[extern isr12_handler]
isr12:
  push rax
  push rcx
  push rdx
  push r8
  push r9
  push r10
  push r11
  call isr12_handler
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdx
  pop rcx
  pop rax
  iretq
  GLOBAL isr12

[extern isr128_handler]
isr128:
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
  mov rdi, rsp
  call isr128_handler
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
  iretq
  GLOBAL isr128

[extern _idt]
idtr:
  dw 4095
  dq _idt

LoadIDT:
  lidt[idtr]
  sti
  ret
  GLOBAL LoadIDT

%macro IncBin 2
  SECTION .rodata
  GLOBAL %1
 %1:
  incbin %2
  db 0
  %1_size: dq %1_size - %1
%endmacro
 
IncBin CursorImage, "fs/img/cursor.tga"
IncBin logo, "fs/img/logo.tga"
IncBin MiniLogo, "fs/img/mini_logo.tga"
IncBin TerminalLogo, "fs/img/terminal.tga"
IncBin Close, "fs/img/close.tga"

;font
IncBin Aa, "fs/font/Aa.tga"
IncBin Ab, "fs/font/Ab.tga"
IncBin Ba, "fs/font/Ba.tga"
IncBin Bb, "fs/font/Bb.tga"
IncBin Ca, "fs/font/Ca.tga"
IncBin Cb, "fs/font/Cb.tga"
IncBin Da, "fs/font/Da.tga"
IncBin Dbb, "fs/font/Db.tga"
IncBin Ea, "fs/font/Ea.tga"
IncBin Eb, "fs/font/Eb.tga"
IncBin Fa, "fs/font/Fa.tga"
IncBin Fb, "fs/font/Fb.tga"
IncBin Ga, "fs/font/Ga.tga"
IncBin Gb, "fs/font/Gb.tga"
IncBin Ha, "fs/font/Ha.tga"
IncBin Hb, "fs/font/Hb.tga"
IncBin Ia, "fs/font/Ia.tga"
IncBin Ib, "fs/font/Ib.tga"
IncBin Jaa, "fs/font/Ja.tga"
IncBin Jbb, "fs/font/Jb.tga"
IncBin Ka, "fs/font/Ka.tga"
IncBin Kb, "fs/font/Kb.tga"
IncBin La, "fs/font/La.tga"
IncBin Lb, "fs/font/Lb.tga"
IncBin Ma, "fs/font/Ma.tga"
IncBin Mb, "fs/font/Mb.tga"
IncBin Na, "fs/font/Na.tga"
IncBin Nb, "fs/font/Nb.tga"
IncBin Oa, "fs/font/Oa.tga"
IncBin Ob, "fs/font/Ob.tga"
IncBin Pa, "fs/font/Pa.tga"
IncBin Pb, "fs/font/Pb.tga"
IncBin Qa, "fs/font/Qa.tga"
IncBin Qb, "fs/font/Qb.tga"
IncBin Ra, "fs/font/Ra.tga"
IncBin Rb, "fs/font/Rb.tga"
IncBin Sa, "fs/font/Sa.tga"
IncBin Sb, "fs/font/Sb.tga"
IncBin Ta, "fs/font/Ta.tga"
IncBin Tb, "fs/font/Tb.tga"
IncBin Ua, "fs/font/Ua.tga"
IncBin Ub, "fs/font/Ub.tga"
IncBin Va, "fs/font/Va.tga"
IncBin Vb, "fs/font/Vb.tga"
IncBin Wa, "fs/font/Wa.tga"
IncBin Wb, "fs/font/Wb.tga"
IncBin Xa, "fs/font/Xa.tga"
IncBin Xb, "fs/font/Xb.tga"
IncBin Ya, "fs/font/Ya.tga"
IncBin Yb, "fs/font/Yb.tga"
IncBin Za, "fs/font/Za.tga"
IncBin Zb, "fs/font/Zb.tga"
IncBin one, "fs/font/one.tga"
IncBin two, "fs/font/two.tga"
IncBin three, "fs/font/three.tga"
IncBin four, "fs/font/four.tga"
IncBin five, "fs/font/five.tga"
IncBin six, "fs/font/six.tga"
IncBin seven, "fs/font/seven.tga"
IncBin eight, "fs/font/eight.tga"
IncBin nine, "fs/font/nine.tga"
IncBin colon, "fs/font/colon.tga"
IncBin zero, "fs/font/zero.tga"
IncBin slash, "fs/font/slash.tga"