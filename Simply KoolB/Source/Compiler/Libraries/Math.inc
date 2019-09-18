' Math.inc - Math functions 

$IfNDef Mathinc
$Define Mathinc

Declare Function Abs Lib "msvcrt.dll" Alias "fabs" Call C (Number As Double) As Double
Declare Function Acos Lib "msvcrt.dll" Alias "acos" Call C (Number As Double) As Double
Declare Function Asin Lib "msvcrt.dll" Alias "asin" Call C (Number As Double) As Double
Declare Function Atan Lib "msvcrt.dll" Alias "atan" Call C (Number As Double) As Double
Declare Function Atn Lib "msvcrt.dll" Alias "atan" Call C (Number As Double) As Double
Declare Function Ceil Lib "msvcrt.dll" Alias "ceil" Call C (Number As Double) As Double
Declare Function Cos Lib "msvcrt.dll" Alias "cos" Call C (Number As Double) As Double
Declare Function Exp Lib "msvcrt.dll" Alias "exp" Call C (Number As Double) As Double
Declare Function Floor Lib "msvcrt.dll" Alias "floor" Call C (Number As Double) As Double
Declare Function Log Lib "msvcrt.dll" Alias "log" Call C (Number As Double) As Double
Declare Function Sin Lib "msvcrt.dll" Alias "sin" Call C (Number As Double) As Double
Declare Function Sqr Lib "msvcrt.dll" Alias "sqrt" Call C (Number As Double) As Double
Declare Function Sqrt Lib "msvcrt.dll" Alias "sqrt" Call C (Number As Double) As Double
Declare Function Tan Lib "msvcrt.dll" Alias "tan" Call C (Number As Double) As Double
Declare Function Timer Lib "msvcrt.dll" Alias "timer" Call C () As Double
Declare Function Pow Lib "msvcrt.dll" Alias "pow" Call C (Base As Double, Exponent As Double) As Double


Function Decrement (Number As Double) As Double
  Result = Number - 1
End Function

Function Increment (Number As Double) As Double
  Result = Number + 1
End Function

Function Frac (Number As Double) As Double
  Result = Number - Floor(Number)
End Function

$End If