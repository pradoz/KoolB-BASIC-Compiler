' System.inc - System functions 

$IfNDef Systeminc
$Define Systeminc

Type SecurityAttributes
  Length As Integer
  SecurityDescriptor As Integer
  InheritHandle As Integer
End Type


Declare Function ChDir Lib "kernel32.dll" Alias "SetCurrentDirectoryA" (Path As String) As Integer
Declare Function Kill Lib "kernel32.dll" Alias "DeleteFileA" (FileName As String) As Integer
Declare Function MessageBox Lib "user32.dll" Alias "MessageBoxA" (WindowHandle As Integer, Text As String, Caption As String, Type As Integer) As Integer
Declare Function CreateDirectory Lib "kernel32.dll" Alias "CreateDirectoryA" (PathName As String, SecurityAttributes As SECURITYATTRIBUTES) As Integer
Declare Function Randomize Lib "msvcrt.dll" Alias "srand" Call C (Seed As Integer) As Integer
Declare Function Rand Lib "msvcrt.dll" Alias "rand" Call C () As Integer
Declare Function Environ$ Lib "msvcrt.dll" Alias "getenv" Call C (Environment As String) As String
Declare Function Rename Lib "kernel32.dll" Alias "MoveFileA" (ExistingFileName As String, NewFileName As String) As Integer
Declare Function RmDir Lib "kernel32.dll" Alias "RemoveDirectoryA" (PathName As String) As Integer
Declare Function Run Lib "msvcrt.dll" Alias "system" Call C (Command As String) As Integer


Function MkDir (Directory As String) As Integer
  Dim SecAttributes As SecurityAttributes
  CreateDirectory(Directory, SecAttributes)
End Function

Function Rnd (Max As Integer) As Integer
  Result = Rand() / 32767 * Max
End Function

Function Sgn (Number As Integer) As Integer
  If Number = 0 Then
    Result = 0
  ElseIf Number > 0 Then
    Result = 1
  ElseIf Number < 0 Then
    Result = -1
  End If
End Function

Function ShowMessage(Message As String) As Integer
  Result = MessageBox(0, Message, "Message:", 0)
End Function

$End If