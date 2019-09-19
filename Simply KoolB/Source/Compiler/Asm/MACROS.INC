;----------High-Level Call
;Usage:
;   ccall FuncName, param1, param2, param 3...
;   pcall FuncName, param1, param2, param 3...
;   stdcall FuncName, param1, param2, param 3...
;   fastcall FuncName, param1, param2, param 3...
%macro pcall 2-9 ;Pascal Call: push first value first, no cleanup
%define _j %1
%rep %0-1
    %rotate 1
    push %1
%endrep
    call _j
%endmacro

%macro ccall 2-9 ;C Call: push last value first, cleanup
%define _j %1
%assign __params %0-1
%rep %0-1
    %rotate -1
    push %1
%endrep
    call _j
    %assign __params __params * 4
    add esp, __params
%endmacro

%macro stdcall 2-9 ;Windows StdCall: push last value first, no cleanup
%define _j %1
%rep %0-1
    %rotate -1
    push %1
%endrep
    call _j
%endmacro

%macro fastcall 2-9 ;Borland FastCall: use registers, no cleanup
%define _j %1
%assign __pnum 1
%rep %0-4
    %rotate -1
    %if __pnum = 1
     mov eax, %1
    %elif __pnum = 2
     mov edx, %1
    %elif __pnum = 3
     mov ebx, %1
    %else
     push %1
    %endif
    %assign __pnum __pnum+1
%endrep
    call _j
%endmacro
