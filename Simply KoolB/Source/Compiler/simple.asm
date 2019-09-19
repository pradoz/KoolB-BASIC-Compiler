;Library functions to import to the program
extern ExitProcess
extern MessageBoxA
extern lstrcat
extern lstrlen
extern lstrcpy
extern HeapCreate
extern HeapAlloc
extern HeapFree
extern GetModuleHandleA
extern GetCommandLineA
extern GetStdHandle
extern lstrlenA
extern lstrcpyA
extern WriteFile
extern Sleep
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
stdcall HeapCreate,0,16384,0
MOV dword[HandleToHeap],EAX
CMP dword[HandleToHeap],0
JNE Label1
JMP NoMemory
Label1:
stdcall GetModuleHandleA,0
MOV dword[Internal_HInstance],EAX
CALL GetCommandLineA
MOV dword[Internal_CommandLine],EAX
stdcall GetStdHandle,-10
MOV dword[HandleToInput],EAX
stdcall GetStdHandle,-11
MOV dword[HandleToOutput],EAX
CMP dword[HandleToInput],-1
JNE Label2
JMP NoConsole
Label2:
CMP dword[HandleToOutput],-1
JNE Label3
JMP NoConsole
Label3:



;Main body of the program where the program runs
MOV EBX,String_1
stdcall lstrlenA,EBX
INC EAX
MOV EDI,EAX
stdcall HeapAlloc,dword[HandleToHeap],8,EDI
CMP EAX,0
JNE Label4
JMP NoMemory
Label4:
MOV EBX,EAX
stdcall lstrcpyA,EBX,String_1
PUSH EBX
POP EBX
stdcall lstrlenA,EBX
INC EAX
DEC EAX
stdcall WriteFile,dword[HandleToOutput],EBX,EAX,ConsoleTemp,0
stdcall HeapFree,dword[HandleToHeap],0,EBX
stdcall WriteFile,dword[HandleToOutput],ConsoleNewLine,1,ConsoleTemp,0
PUSH dword[Number_2+4]
PUSH dword[Number_2]
POP dword[TempQWord1]
POP dword[TempQWord1+4]
MOV dword[TempQWord2],1000
FINIT
FLD qword[TempQWord1]
FILD dword[TempQWord2]
FMUL ST0,ST1
FRNDINT
FIST dword[TempQWord1]
PUSH dword[TempQWord1]
POP EBX
stdcall Sleep,EBX



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

NoMemory:
MOV dword[ExitStatus],1
stdcall MessageBoxA,0,NoMemMessage,Error,0
JMP Exit

NoLibrary:
MOV dword[ExitStatus],1
POP EBX
stdcall lstrlen,EBX
ADD EAX,30
stdcall HeapAlloc,dword[HandleToHeap],8,EAX
MOV EDI,EAX
stdcall lstrcpy,EDI,NoLibFound
stdcall lstrcat,EDI,EBX
stdcall MessageBoxA,0,EDI,Error,0
stdcall HeapAlloc,dword[HandleToHeap],8,EDI
JMP Exit

NoFunction:
MOV dword[ExitStatus],1
POP EBX
stdcall lstrlen,EBX
ADD EAX,30
stdcall HeapAlloc,dword[HandleToHeap],8,EAX
MOV EDI,EAX
stdcall lstrcpy,EDI,NoFunctionFound
stdcall lstrcat,EDI,EBX
JMP Exit
NoConsole:
MOV dword[ExitStatus],1
stdcall MessageBoxA,0,NoConsoleMessage,Error,0
JMP Exit



;Data section of the program
section .data
NoMemMessage db "Could not allocate memory.",0
NoLibFound db "Could not find library: ",0
NoFunctionFound db "Cound not find function: ",0
Error db "An error occured",0
ParameterPool dd 0
HandleToHeap dd 0
TempQWord1 dq 0.0
TempQWord2 dq 0.0
True dd -1
Internal_HInstance dd 0
Internal_CommandLine dd 0
NoConsoleMessage db 'Error - Cannot access the console handles for Input/Output.',0
ConsoleTemp dd 0
ConsoleNewLine db 10,0
ConsoleClear db 'CLS',0
HandleToInput dd 0
HandleToOutput dd 0
String_1 db "HELLO WE FREAKING COMPILED",0
Number_2 dq 3.0
ExitStatus dd 0

