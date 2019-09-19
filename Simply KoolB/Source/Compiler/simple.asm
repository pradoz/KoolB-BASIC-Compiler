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
CMP dword[HandleToHeap],1
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
stdcall HeapAlloc,dword[HandleToHeap],8,1
CMP EAX,0
JNE Label4
JMP NoMemory
Label4:
MOV dword[Scope0__EMPTYLINE_String],EAX
MOV byte[EAX],0



;Main body of the program where the program runs
stdcall lstrlenA,dword[Scope0__EMPTYLINE_String]
INC EAX
MOV EBX,EAX
stdcall HeapAlloc,dword[HandleToHeap],8,EBX
CMP EAX,0
JNE Label5
JMP NoMemory
Label5:
MOV EDI,EAX
stdcall lstrcpyA,EDI,dword[Scope0__EMPTYLINE_String]
PUSH EAX
POP EBX
stdcall lstrlenA,EBX
INC EAX
DEC EAX
stdcall WriteFile,dword[HandleToOutput],EBX,EAX,ConsoleTemp,0
stdcall HeapFree,dword[HandleToHeap],0,EBX
stdcall WriteFile,dword[HandleToOutput],ConsoleNewLine,1,ConsoleTemp,0
stdcall lstrlenA,dword[Scope0__EMPTYLINE_String]
INC EAX
MOV EBX,EAX
stdcall HeapAlloc,dword[HandleToHeap],8,EBX
CMP EAX,0
JNE Label6
JMP NoMemory
Label6:
MOV EDI,EAX
stdcall lstrcpyA,EDI,dword[Scope0__EMPTYLINE_String]
PUSH EAX
POP EBX
stdcall lstrlenA,EBX
INC EAX
DEC EAX
stdcall WriteFile,dword[HandleToOutput],EBX,EAX,ConsoleTemp,0
stdcall HeapFree,dword[HandleToHeap],0,EBX
stdcall WriteFile,dword[HandleToOutput],ConsoleNewLine,1,ConsoleTemp,0
stdcall lstrlenA,dword[Scope0__EMPTYLINE_String]
INC EAX
MOV EBX,EAX
stdcall HeapAlloc,dword[HandleToHeap],8,EBX
CMP EAX,0
JNE Label7
JMP NoMemory
Label7:
MOV EDI,EAX
stdcall lstrcpyA,EDI,dword[Scope0__EMPTYLINE_String]
PUSH EAX
POP EBX
stdcall lstrlenA,EBX
INC EAX
DEC EAX
stdcall WriteFile,dword[HandleToOutput],EBX,EAX,ConsoleTemp,0
stdcall HeapFree,dword[HandleToHeap],0,EBX
stdcall WriteFile,dword[HandleToOutput],ConsoleNewLine,1,ConsoleTemp,0
MOV EBX,String_1
stdcall lstrlenA,EBX
INC EAX
MOV EDI,EAX
stdcall HeapAlloc,dword[HandleToHeap],8,EDI
CMP EAX,0
JNE Label8
JMP NoMemory
Label8:
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



;Prepare the program to exit, then terminate the program
Exit:
stdcall HeapFree,dword[HandleToHeap],0,dword[Scope0__EMPTYLINE_String]
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
stdcall MessageBoxA,0,EDI,Error,0
stdcall HeapAlloc,dword[HandleToHeap],8,EDI
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
Scope0__EMPTYLINE_String dd 0
String_1 db "HELLO WE FREAKING COMPILED",0
ExitStatus dd 0

