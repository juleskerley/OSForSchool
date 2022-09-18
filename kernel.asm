GLOBAL k_printstr

k_printstr:
    push ebp
    mov ebp, esp
    pushf
    push esi ; string mem location
    push edi ; vga mem location
    push eax ; offset

    mov edi, 0xB8000h
    mov esi, [ebp+8]
    ; I think it's +12 because the string is the *first* thing into the function

    imul eax, [ebp+12], 80 ; Using ebp to slot in row
    add  eax, [ebp+16] ; same here for columns
    imul eax, eax, 2 ; I think there is a better way but I forgot...

    add edx, eax ; adding offset to starting location

    loop:
        cmp BYTE [esi], 0 ; string is 0 bytes big left
        je loop_end
        cmp edx, 0xB8FA0 ;   0xB000+FA0 = 4000 = 25*80*2
        jg loop_end
        movsb ; gets the character and puts it into mem for video and incs
        mov BYTE [edi], 31h
        inc edi
        jmp loop
    loop_end:
        pop ecx
        pop ebx
        pop eax
        pop edi
        pop esi
        popf
        pop ebp
        ret
