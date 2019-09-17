;Library functions to import to the KoolB program
extern ExitProcess
extern HeapDestroy



;Initialize everything to prepare the program to run
section .text
%include "./Asm/MACROS.INC"
global START
START:
PUSH EBP
MOV EBP,ESP
PUSH EBX
PUSH ESI
PUSH EDI



;Main body of the program where the program runs



;Prepare the program to exit, then terminate the program
Exit:
stdcall HeapDestroy,dword[HandleToHeap]
POP EDI
POP ESI
POP EBX
MOV ESP,EBP
POP EBP
MOV EAX,dword[ExitStatus]
stdcall ExitProcess,dword[ExitStatus]
RET




;Data section of the KoolB programp
section .data

