; Presenting objects from to external programs
GLOBAL k_printstr
GLOBAL go
GLOBAL dispatch
GLOBAL lidtr
; Pulling from objects from external programs
EXTERN dequeue
EXTERN enqueue

EXTERN readyQueue

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
    push readyQueue
    call dequeue
    add esp, 4 ; cleans the stack of that pushed address
    ; putting the return into the global variable
    mov DWORD BYTE [curProc], eax ; call, I think, puts the return into eax
    mov eax, [curProc] ; This is not needed here but I will do it every time
    ; don't need to put a jmp, it'll fall to go_rest
    ; I think this is working!!

go_rest:
    ; so future me, this isn't correct, but accessing the struct might be like
    mov esp, DWORD BYTE [eax+4] ; Because this is the second entry in the struct
    ; and the first two struct values are uint32_t (which are == in size to dw
    ; therefore it is 4
    popf
    popa
    ret

dispatch:
    call yield

dispatch_leave: 
; iret usage implies this is for interrupts so that might be a phase 2 thing?
    iret

yield:
    pusha
    pushf
    mov eax, [curProc]
    mov [eax+4], esp
    push eax
    push readyQueue
    call enqueue
    call dequeue ; This takes advantage of the fact that readyQueue is the first thing on the stack
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
