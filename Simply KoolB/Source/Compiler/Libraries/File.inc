' File.inc - File IO functions 

$IfNDef Fileinc
$Define Fileinc

$Include "String.inc"

Declare Function fopen Lib "Msvcrt.dll" Alias "fopen" Call C (FileName As String, Mode As String) As Integer
Declare Function fwrite Lib "Msvcrt.dll" Alias "fwrite" Call C (Buffer As String, Size As Integer, Count As Integer, File As Integer) As Integer
Declare Function fwrite2 Lib "Msvcrt.dll" Alias "fwrite" Call C (Buffer As Integer, Size As Integer, Count As Integer, File As Integer) As Integer
Declare Function fputs Lib "Msvcrt.dll" Alias "fputs" Call C (Buffer As String, File As Integer) As Integer
Declare Function fread Lib "Msvcrt.dll" Alias "fread" Call C (byref Buffer As String, Size As Integer, Count As Integer, File As Integer) As Integer
Declare Function fread2 Lib "Msvcrt.dll" Alias "fread" Call C (Buffer As Integer, Size As Integer, Count As Integer, File As Integer) As Integer
Declare Function fgets Lib "Msvcrt.dll" Alias "fgets" Call C (byref Buffer As String, Max As Integer, File As Integer) As Integer
' Declare Function putw Lib "Msvcrt.dll" Alias "putw" Call C (Int As Integer, File As Integer) As Integer
Declare Function ftell Lib "Msvcrt.dll" Alias "ftell" Call C (File As Integer) As Integer
Declare Function fseek Lib "Msvcrt.dll" Alias "fseek" Call C (File As Integer, Offset As Integer, Origin As Integer) As Integer
Declare Function fgetpos Lib "Msvcrt.dll" Alias "fgetpos" Call C (File As Integer, Pos As Integer) As Integer
Declare Function fsetpos Lib "Msvcrt.dll" Alias "fsetpos" Call C (File As Integer, Pos As Integer) As Integer
Declare Function fclose Lib "Msvcrt.dll" Alias "fclose" Call C (File As Integer) As Integer

$Const FileRead = "r"
$Const FileCreateWrite = "w"
$Const FileAppend = "a"
$Const FileReadWrite = "r+"
$Const FileCreateReadWrite = "w+"
$Const FileReadAppend = "a+"

' <file> = FileOpen(<filename>, <access flags>)
Function FileOpen(FileName As String, Flags As String) As Integer
  Result = fopen(FileName, Flags + "t")
End Function

' FileWriteStr(<file>, <str>)
Sub FileWriteStr(File As Integer, S As String)
  fputs(S, File)
End Sub

' <string> = FileReadStr(<file>, <length>)
Function FileReadStr(File As Integer, Length As Integer) As String
  Dim S As String
  S = Space$(Length+1)
  fread(S,Length,1,File)
  Result = S
End Function

' FileWriteLine(<file>, <string>)
Sub FileWriteLine(File As Integer, S As String)
  fputs(S + Chr$(13) + Chr$(10), File)
End Sub

' <FileSize> = FileGetSize(<file>)
Function FileGetSize(File As Integer) As Integer
  Dim CurPos As Integer
  CurPos = ftell(File)
  fseek(File, 0, 2)
  Result = ftell(File)
  fseek(File, 0, CurPos)
End Function

Function FileCountLines(File As Integer) As Integer
  Dim S As String
  S = Space(GetFileSize
  Result = 0
  
End Function

Function FileReadLine

' <string> = FileReadLine(<file>)
Function FileReadLine(File As Integer) As String
  Dim S As String
  S = Space$(FileGetSize(File)+1)
  fgets(S, FileGetSize(File) + 1, File)
  Result = S
End Function

' FileWriteInt(<file>, <number>)
Sub FileWriteInt(File As Integer, Number As Integer)
  fwrite2(AddressOf(Number), 4, 1, File)
End Sub

' <number> = FileReadInt(<file>)
Function FileReadInt(File As Integer) As Integer
  fread2(AddressOf(Result), 4, 1, File)
End Function

' FileWriteDouble(<file>, <number>)
Sub FileWriteDouble(File As Integer, Number As Double)
  fwrite2(AddressOf(Number), 8, 1, File)
End Sub

' <number> = FileReadDouble(<file>)
Function FileReadDouble(File As Integer) As Double
  fread2(AddressOf(Result), 8, 1, File)
End Function

' FileSetPos(<file>, <position to start reading>)
Sub FileSetPos(File As Integer, Pos As Integer) 
  'fsetpos(File, Pos)
  fseek(File, Pos, 0)
End Sub

' <pos> = FileGetPos(<file>)
Function FileGetPos(File As Integer) As Integer
  'fgetpos(File, AddressOf(Result))
  Result = ftell(File)
End Function

' FileClose(<file>)
Sub FileClose(File As Integer) 
  fclose(File)
End Sub

$End If