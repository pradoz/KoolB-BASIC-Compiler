' Misc.inc - Miscellaneous functions 

$IfNDef Miscinc
$Define Miscinc

Function Or(N1 As Integer, N2 As Integer) As Integer
  $Asm
    MOV EBX,dword[EBP+8]
    MOV EDI,dword[EBP+12]
    OR  EBX,EDI
    MOV dword[EBP-4],EBX
  $End ASm
End Function

Function BOr(N1 As Integer, N2 As Integer) As Integer
  $Asm
    MOV EBX,dword[EBP+8]
    MOV EDI,dword[EBP+12]
    OR  EBX,EDI
    MOV dword[EBP-4],EBX
  $End ASm
End Function

Function BAnd(N1 As Integer, N2 As Integer) As Integer
  $Asm
    MOV EBX,dword[EBP+8]
    MOV EDI,dword[EBP+12]
    AND EBX,EDI
    MOV dword[EBP-4],EBX
  $End ASm
End Function

Function BXor(N1 As Integer, N2 As Integer) As Integer
  $Asm
    MOV EBX,dword[EBP+8]
    MOV EDI,dword[EBP+12]
    XOR EBX,EDI
    MOV dword[EBP-4],EBX
  $End ASm
End Function

$End If
