' Convert.inc - Convert functions 

$IfNDef Convertinc
$Define Convertinc

$Include "String.inc"

Declare Function gcvt Lib "msvcrt.dll" Alias "_gcvt" Call C (Number As Double, NumDigits As Integer, ByRef S As String) As Integer
Declare Function Val Lib "msvcrt.dll" Alias "atof" Call C (S As String) As Double

Function Str$(Number As Double) As String
  Dim NumDigits As Integer
  NumDigits = 100
  Result = Space$(NumDigits + 1)
  gcvt(Number, NumDigits, Result)
End Function

$End If