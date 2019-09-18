' Console.inc - Console functions

$IfNDef Consoleinc
$Define Consoleinc

$Include "Misc.inc"

Declare Function GetStdHandle Lib "Kernel32.dll" Alias "GetStdHandle" (N As Integer) As Integer
Declare Function SetConsoleTitleWinAPI Lib "Kernel32.dll" Alias "SetConsoleTitleA" (S As String) As Integer
Declare Function SetConsoleTextAttribute Lib "Kernel32.dll" _
                 Alias "SetConsoleTextAttribute" Call Std (hConsoleOutput As Integer, _
                 wAttributes As Integer) As Integer


$Const BACKGROUND_BLUE = 16
$Const BACKGROUND_GREEN = 32
$Const BACKGROUND_RED = 64
$Const BACKGROUND_INTENSITY = 128
$Const FOREGROUND_BLUE = 1
$Const FOREGROUND_GREEN = 2
$Const FOREGROUND_RED = 4
$Const FOREGROUND_INTENSITY = 8

Sub Color (Foreground As Integer, Background As Integer)
  SetConsoleTextAttribute(GetStdHandle(-11), Or(Foreground, Background))
End Sub

Function Cslrin () As Integer

End Function

Function Get$(N As Integer) As String

End Function

Function InKey$() As String

End Function

Sub Locate(Row As Integer, Column As Integer)
  $Asm
    extern SetConsoleCursorPosition
    extern GetStdHandle
    MOV AX,word[EBP+12]
    PUSH AX
    MOV AX,word[EBP+8]
    PUSH AX
    stdcall GetStdHandle,-11
    PUSH EAX
    CALL SetConsoleCursorPosition
  $End Asm
End Sub

Function Pos() As Integer

End Function

Function GetConsoleReadHandle() As Integer
  Result = GetStdHandle(-10)
End Function

Function GetconsoleWriteHandle() As Integer
  Result = GetStdHandle(-11)
End Function

Sub SetConsoleTitle(Title As String)
  SetConsoleTitleWinAPI(Title)
End Sub

$End If