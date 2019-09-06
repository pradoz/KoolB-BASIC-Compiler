;Library functions to import to the KoolB program
extern ExitProcess



;Initialize everything to prepare the program to run
section .text
%include "Asm\MACROS.INC"
global START
START:



;Main body of the program where the program runs



;Prepare the program to exit, then terminate the program
Exit:
stdcall ExitProcess,0



;Data section of the KoolB programp
section .data

