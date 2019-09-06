;Library functions to import to the KoolB compiler
extern ExitProcess    ; Add function ExitProcess to the program


;Initialize everything to prep the app to run
section .text         ; The section where all our code is held
%include "Asm\MACROS.INC" ; Include all the macros we need
global START          ; Make sure the linker can find where the program starts
START:                ; Where the program begins


;The main body of the app where the app actually runs


;Prepare the app to exit and then terminate the app
Exit:                 ; Where everything winds down and the program prepares to exit
stdcall ExitProcess,0 ; Exit normally from the program


;Data section of the KoolB app
section .data         ; The section where all our data for variables are held 