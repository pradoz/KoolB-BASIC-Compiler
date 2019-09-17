' String manipulation using the standard library

$AppType Console
$Optimize On
$Compress On

$Include "Libraries/KoolB.inc"

Input "Enter your name: "; A$

If Instr(1, UCase$(A$), Ucase$("Hello")) <> 0 Then
  Print "Hey, we have the same name!"
Else
  Print "Well, nice knowing you"
End If

Sleep 2

Print ""
Print ""

Input "Enter random phrase: "; A$

Dim Length As Integer
Length = Len(A$)

Sleep 1
Print "Length of your random phrase: "; Length
Sleep 1
Print "Uppercase rendition of your phrase: "; UCase$(A$)
Sleep 1
Print "Lowercase rendition of your phrase: "; LCase$(A$)
Sleep 1
Print "Middle 3 letters of your phrase: "; Mid$(A$, Length/2, 3)

Print "" 
Print ""

Sleep 2

Input "Enter a number between 1-255: "; A&
Sleep 1
Print "Your number is ASCII character: "; Chr$(A&)

Print "" 
Print ""

Sleep 2

Print "Look in Libraries\String.inc for more functions related to strings."

Print "" 

Sleep 4

Print "Bye now!"

Sleep 2