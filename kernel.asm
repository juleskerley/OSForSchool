; Presenting objects from to external programs
GLOBAL k_printstr
GLOBAL go
GLOBAL dispatch
GLOBAL dispatch_leave
GLOBAL lidtr
GLOBAL init_timer_dev
GLOBAL outportb
; Pulling from objects from external programs
EXTERN dequeue_priority
EXTERN enqueue_priority

EXTERN lowPriorityQueue

GLOBAL curProc ; pointer to current running process
curProc dd 0

k_printstr:
    push ebp
    mov ebp, esp
    pushf
    push esi ; string mem location
    push edi ; vga mem location
    push eax ; offset

    mov DWORD edi, 0xB8000 ; vga applied
    mov esi, [ebp+8] ; string applied
    imul eax, [ebp+12], 80 ; Using ebp to fill in row
    add  eax, [ebp+16] ; same here for columns
    imul eax, eax, 2 ; I think there is a better way but I forgot...

    add edi, eax ; adding offset to starting location

    loop:
        cmp BYTE [esi], 0 ; string is 0 bytes big left
        je loop_end
        cmp DWORD edi, 0xB8FA0 ;   0xB000+FA0 = 4000 = 25*80*2
        jg loop_end
        movsb ; gets the character and puts it into mem for video and incs
        mov BYTE [edi], 31
        inc edi
        jmp loop
    loop_end:
        pop eax
        pop edi
        pop esi
        popf
        pop ebp
        ret

go:
    push lowPriorityQueue
    call dequeue_priority
    add esp, 4 ; cleans the stack of that pushed address
    ; putting the return into the global variable
    mov DWORD BYTE [curProc], eax ; call, I think, puts the return into eax
    mov eax, [curProc] ; This is not needed here but I will do it every time
    ; don't need to put a jmp, it'll fall to go_rest

go_rest:
    mov esp, DWORD BYTE [eax+4] ; Because this is the second entry in the struct
    ; and the first two struct values are uint32_t (which are == in size to dw
    ; therefore it is 4
    popa
    ret

dispatch:
    call yield

dispatch_leave:
    push eax
    mov al, 0x20
    out 0x20, al
    pop eax
    iret

yield:
    pusha
    mov eax, [curProc]
    mov [eax+4], esp
    push eax
    push lowPriorityQueue
    call enqueue_priority
    call dequeue_priority ; This takes advantage of the fact that readyQueue is the first thing on the stack
    ; I do not have to push it back into the stack
    add esp, 8
    mov DWORD BYTE [curProc], eax ; call, I think, puts the return into eax
    mov eax, [curProc] ; This is not needed here but I will do it every time
    jmp go_rest

lidtr:
    push ebp
    mov ebp, esp
    push eax
    mov eax, [ebp+8]
    lidt [eax]
    pop eax
    pop ebp
    ret

init_timer_dev:
    ; Basic setup, step 1
    push ebp
    mov ebp, esp
    push eax
    push edx
    ; move ms into value, step 2
    mov dx, [ebp+8]
    ; multiplying dx by 1193
    imul dx, dx, 1193

    ; Dr. Rogers code:
    mov al, 0b00110110 ; 0x43 is the Write control word
    out 0x43, al
    ; 5) Load the LSB first then the MSB.
    ; 0x40 = Load counter 0 with the following code: 
    mov ax, dx
    out 0x40, al ; LSB
    xchg ah, al
    out 0x40, al ; MSB
    ; 6) clean up (pop ebp and other regs used) and return, my code:
    pop edx
    pop eax
    pop ebp
    ret


outportb:
    push ebp
    mov ebp, esp
    push eax
    push edx
    mov dx, [ebp+8]
    mov al, [ebp+12]
    out dx, al
    pop edx
    pop eax
    pop ebp
    ret
