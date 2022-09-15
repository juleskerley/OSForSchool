GLOBAL k_printstr

k_printstr:
    push ebp
    mov ebp, esp
    pushf
    push esi ; string pointer
    push eax ; offset
    push ebx ; row
    push ecx ; column

    mov esi, [ebp+12]
    ; I think it's +12 because the string is the *first* thing into the function
    mov ebx, [ebp+8]
    mov ecx, [ebp+4]


    loop:
        cmp [STRING], 00 ; string is 0 bytes big left
        je loop_end
        cmp [STRING], [MAXMEMLOCATIONINVIDEO]
        jg loop_end
        movsb ; gets the character and puts it into mem for video and incs
        mov WHITETEXTBLUEBACKGROUND, [CURMEMLOCATION]
        inc [CURMEMLOCATION]
        jmp loop
    loop_end:
        pop ecx
        pop ebx
        pop eax
        pop esi
        popf
        pop ebp
        ret
