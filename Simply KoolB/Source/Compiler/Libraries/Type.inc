' Type.inc - Type functions 

$IfNDef Typeinc
$Define Typeinc

Declare Function IsAlNum Lib "msvcrt.dll" Alias "isalnum" Call C (Char As Integer) As Integer
Declare Function IsAlpha Lib "msvcrt.dll" Alias "isalpha" Call C (Char As Integer) As Integer
Declare Function IsCntrl Lib "msvcrt.dll" Alias "iscntrl" Call C (Char As Integer) As Integer
Declare Function IsDigit Lib "msvcrt.dll" Alias "isdigit" Call C (Char As Integer) As Integer
Declare Function IsGraph Lib "msvcrt.dll" Alias "isgraph" Call C (Char As Integer) As Integer
Declare Function IsLower Lib "msvcrt.dll" Alias "islower" Call C (Char As Integer) As Integer
Declare Function IsPrint Lib "msvcrt.dll" Alias "isprint" Call C (Char As Integer) As Integer
Declare Function IsPunct Lib "msvcrt.dll" Alias "ispunct" Call C (Char As Integer) As Integer
Declare Function IsSpace Lib "msvcrt.dll" Alias "isspace" Call C (Char As Integer) As Integer
Declare Function IsUpper Lib "msvcrt.dll" Alias "isupper" Call C (Char As Integer) As Integer
Declare Function IsXDigit Lib "msvcrt.dll" Alias "isxdigit" Call C (Char As Integer) As Integer

Declare Function ToLower Lib "msvcrt.dll" Alias "tolower" Call C (Char As Integer) As Integer
Declare Function ToUpper Lib "msvcrt.dll" Alias "toupper" Call C (Char As Integer) As Integer

$End If