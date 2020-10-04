;Вариант №17
;Условие: сначала вводятся элементы массива, а потом нужно получить новый массив по следующим правилам:
;Взять из массива А все элементы, расположенные после последнего положительного.

format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        tmpStack dd ?

        repEnter db 'The wrong size of array.', 10, 13, 0

        enterN db 'Enter size of the array:', 10, 13, '>> ', 0

        endl db 10, 13, 0

        info db "Making second array...", 10, 13, 0

        enterStart db 'Enter the ', 0

        enterEnd db ' elemet:', 10, 13, '>> ', 0

        outStart db '[ ', 0

        outEnd db ' ]', 10, 13, 0

        newStr db '', 10, 13, 0

        outMiddle db ', ', 0, 10, 13,0

        lastMessage db 'The program has finished.',0
        lastPos db 'Index of last positive: ',0
        Nob db 'B = [].',0

        arrA db 'A = ', 0

        arrB db 'B = ', 0

        hheap dd ?

        hheap1 dd ?

        mas dd ?

        mas1 dd ?

        i dd ?

        null dd 0

        digit db '%d', 0

        N dd ?
        N1 dd 5

section '.code' code readable executable

        start:
                call InitializingArray
                call InputOfTheArray
                call PrintFirstArray
                call FindLastPos
                call InitializingSecondArray
                call FillSecondArray
                call PrintSecondArray
                call Finish

                cinvoke printf,lastMessage
                call [getch]
                push [null]
                call [ExitProcess]
;---------------------------------------------------------
InitializingArray:
                mov [tmpStack], esp
                push enterN
                call [printf]
                push N
                push digit
                call [scanf]
                cmp [N],1
                jl repeatInput
                cmp [N],1000000
                jg repeatInput
                invoke HeapCreate,HEAP_NO_SERIALIZE,0,0
                mov [hheap],eax
                invoke HeapAlloc,[hheap],HEAP_NO_SERIALIZE,N*8
                mov [mas],eax

                cinvoke printf,newStr
                mov esp, [tmpStack]
                mov [i], -1
                ret

repeatInput:
        cinvoke printf,newStr
        cinvoke printf,repEnter
        jmp start
;---------------------------------------------------------
InputOfTheArray:
                mov [tmpStack], esp
                jmp loop2
                loop2:

                        add [i], 1

                        cinvoke printf,enterStart
                        cinvoke printf,digit,[i]
                        cinvoke printf,enterEnd
                        mov ecx, [i]
                        imul ecx, dword 4
                        mov eax, [mas]
                        add eax, ecx
                        cinvoke scanf, digit, eax

                        mov ecx, [i]
                        add ecx, 1
                        cmp ecx, [N]
                        jne loop2
                        jmp end2

                end2:

                        cinvoke printf,newStr
                        cinvoke printf, arrA
                        cinvoke printf,outStart
                        mov [i], -1
                 mov esp, [tmpStack]
                 ret
;----------------------------------------------------------
PrintFirstArray:
        mov [tmpStack], esp
               loop3:
                        add [i], 1
                        mov ecx, [i]
                        imul ecx, dword 4
                        mov eax, [mas]
                        add eax, ecx
                        cinvoke printf, digit, dword [eax]

                        mov ecx, [i]
                        add ecx, 1
                        cmp ecx, [N]
                        jne printDot
                        jmp loop3cont

                printDot:

                        cinvoke printf,outMiddle

                loop3cont:

                        mov ecx, [i]

                        add ecx, 1

                        cmp ecx, [N]

                        jne loop3

                        jmp end3
                end3:
                        cinvoke printf,outEnd
                        cinvoke printf,newStr
                        mov [i], -1
        mov esp, [tmpStack]
        ret
;----------------------------------------------------------
FindLastPos:
        mov [tmpStack], esp
        cinvoke printf, lastPos
        mov edi, -1
                loop5:

                        add [i], 1
                        mov ecx, [i]
                        imul ecx, dword 4
                        mov eax, [mas]
                        add eax, ecx
                        mov ebx, [eax]
                        cmp ebx,0
                        jg positive
                        continueLoop5:
                        mov ecx, [i]
                        add ecx, 1
                        cmp ecx, [N]
                        jne loop5
                        jmp end5

                positive:
                        mov edi, [i]
                        jmp continueLoop5

                end5:
                        cinvoke printf, digit, edi
                        cinvoke printf, newStr
                        mov [i], -1

        cmp edi, -1
        je NoPos
        mov esp, [tmpStack]
        add edi, 1
        cmp edi,[N]
        je NoB
        sub edi, 1
        ret
NoPos:
        mov edi, -1
        jmp InitializingSecondArray
;---------------------------------------------------------
InitializingSecondArray:
                mov [tmpStack], esp

                mov edx, [N]
                sub edx, edi
                sub edx, 1

                mov [N1], edx
                invoke HeapCreate,HEAP_NO_SERIALIZE,0,0
                mov [hheap1],eax
                invoke HeapAlloc,[hheap1],HEAP_NO_SERIALIZE,N1*8
                mov [mas1],eax
                mov [i], -1

                mov esp, [tmpStack]
                ret
;---------------------------------------------------------
NoB:
                cinvoke printf, Nob
                invoke HeapFree,[hheap],HEAP_NO_SERIALIZE, [mas]
                mov esp, [tmpStack]
                cinvoke printf, newStr
                cinvoke printf,lastMessage
                call [getch]
                push [null]
                call [ExitProcess]
;----------------------------------------------------------
FillSecondArray:
        mov [tmpStack], esp
        cinvoke printf, newStr
        cinvoke printf,info
        mov [i], edi
        add edi, 1
                loop4:
                        add [i], 1
                        mov ecx, [i]
                        mov ebx, [i]
                        sub ebx, edi
                        imul ecx, dword 4
                        imul ebx, dword 4

                        mov eax, [mas]
                        mov edx, [mas1]

                        add eax, ecx
                        add edx, ebx
                        mov esi, [eax]

                        mov [edx], esi

                        mov ecx, [i]

                        add ecx, 1
                        cmp ecx, [N]

                        jne loop4

                        jmp end4

                end4:
                        cinvoke printf, endl
                        mov [i], -1
                        cinvoke printf, arrB
                        cinvoke printf,outStart
        mov esp, [tmpStack]
        ret
;----------------------------------------------------------

PrintSecondArray:
        mov [tmpStack], esp
               loop6:
                        add [i], 1
                        mov ecx, [i]
                        imul ecx, dword 4
                        mov eax, [mas1]
                        add eax, ecx
                        cinvoke printf, digit, dword [eax]
                        mov ecx, [i]
                        add ecx, 1
                        cmp ecx, [N1]
                        jne printDot6
                        jmp loop6cont

                printDot6:

                        cinvoke printf,outMiddle

                loop6cont:

                        mov ecx, [i]
                        add ecx, 1
                        cmp ecx, [N1]
                        jne loop6
                        jmp end6
                end6:
                        cinvoke printf,outEnd
                        cinvoke printf,newStr
                        mov [i], -1
        mov esp, [tmpStack]
        ret

;---------------------------------------------------------
Finish:
        mov [tmpStack], esp
        invoke HeapFree,[hheap],HEAP_NO_SERIALIZE, [mas]
        invoke HeapFree,[hheap1],HEAP_NO_SERIALIZE, [mas1]
        mov esp, [tmpStack]
        ret

section 'idata' import data readable

        library kernel, 'kernel32.dll',\
               msvcrt, 'msvcrt.dll',\
               user32,'USER32.DLL'
               include 'api\user32.inc'
               include 'api\kernel32.inc'

        import kernel,\
               ExitProcess, 'ExitProcess',\
               HeapCreate, 'HeapCreate',\
               HeapFree, 'HeapFree',\
               HeapAlloc, 'HeapAlloc'
               include 'api\kernel32.inc'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'
