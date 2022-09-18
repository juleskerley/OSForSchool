GLOBAL k_printstr

vgaStart    DWORD PTR 0xB8000h
k_printstr:
    push ebp
    mov ebp, esp
    pushf
    push esi ; string mem location
    push edi ; vga mem location
    push eax ; offset
    push ebx ; row
    push ecx ; column

    mov edi, vgaStart
    mov [esi], [ebp+12]
    ; I think it's +12 because the string is the *first* thing into the function
    mov ebx, [ebp+8]
    mov ecx, [ebp+4]

   

    loop:
        cmp BYTE [esi], 0 ; string is 0 bytes big left
        je loop_end
        cmp [esi+eax], [vgaStart+4000] ; 4000 = 25*80*2
        jg loop_end
        movsb ; gets the character and puts it into mem for video and incs
        mov 31h, [edi]
        inc edi
        jmp loop
    loop_end:
        pop ecx
        pop ebx
        pop eax
        pop esi
        popf
        pop ebp
        ret
