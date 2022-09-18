GLOBAL k_printstr

k_printstr:
    push ebp
    mov ebp, esp
    pushf
    push esi ; string mem location
    push edi ; vga mem location
    push eax ; offset
    push ebx ; row
    push ecx ; column

    mov edi, 0xB8000
    mov esi, [ebp+12]
    ; I think it's +12 because the string is the *first* thing into the function
    mov ebx, [ebp+8]
    mov ecx, [ebp+4]

    imul eax, ebx, 80 ; maybe change it so that it is [ebp+8]?
    add  eax, ecx ; same here
    imul eax, eax, 2 ; I think there is a better way but I forgot...

    add esi, eax

    loop:
        cmp BYTE [esi], 0 ; string is 0 bytes big left
        je loop_end
        cmp esi, 0xB8FA0 ;   0xB000+FA0 = 4000 = 25*80*2
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
