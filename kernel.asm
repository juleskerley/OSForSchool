; Presenting objects from to external programs
GLOBAL k_printstr
GLOBAL go
; Pulling from objects from external programs
EXTERN dequeue

GLOBAL curProc ; pointer to current running process
curProc DW 0

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
    push ebp
    mov ebp, esp
    pushf
    push eax
    ;pushes the point on stack with the queue back onto the stack so the call works 
    push DWORD [ebp+8]
    call dequeue
    add 4, esp ; cleans the stack of that pushed address
    ; putting the return into the global variable
    mov DWORD BYTE [curProc], eax ; call, I think, puts the return into eax
    ; don't need to put a jmp, it'll fall to go_rest

go_rest:
    ; so future me, this isn't correct, but accessing the struct might be like
    mov [curProc+8], esp ;like this, figure it out pls

dispatch:
    jmp yield

dispatch_leave: ; iret usage implies this is for interrupts so that might be a phase 2 thing?
    iret

yield:

    ret
