' Time.inc - Time functions

$IfNDef Timeinc
$Define Timeinc

$Include "Convert.inc"

Type SYSTEMTIME
  wYear As Integer
  wMonth As Integer
  wDayOfWeek As Integer
  wDay As Integer
  wHour As Integer
  wMinute As Integer
  wSecond As Integer
  wMilliseconds As Integer
End Type

Type TM
  Hour As Integer
  IsDST As Integer
  MDay As Integer
  Min As Integer
  Mon As Integer
  Sec As Integer
  WDay As Integer
  YDay As Integer
  Year As Integer
End Type

Declare Function GetLocalTime Lib "kernel32.dll" Alias "GetLocalTime" (byref lpSystemTime As SYSTEMTIME) As Integer
Declare Function GetLastError Lib "kernel32.dll" Alias "GetLastError" () As integer

  Dim ReturnTime As SystemTime

Sub Time$()
  print GetLocalTime(ReturnTime)
  print GetLastError()
  print ReturnTime.wMilliseconds
End Sub

'Declare Function LocalTime Lib "MSVCRT.DLL" Alias "localtime" Call C(Time As Integer) As Integer
'Declare Function Time Lib "MSVCRT.DLL" Alias "time" Call C (Time As Integer) As Integer
'Declare Function AscTime Lib "MSVCRT.DLL" Alias "asctime" Call C (Time As Integer) As Integer
'
'Dim GlobalTM As TM, GlobalTime As Integer
'
'Sub FillGobalTM(TMPointer As Integer, GTM As Integer)
'  $Asm
'    EXTERN RtlMoveMemory
'    MOV EBX,dword[EBP+8]
'    MOV ESI,dword[EBP+12]
'    stdcall RtlMoveMemory,dword[ESI],dword[EBX],36
'  $End Asm
'  GlobalTM.Hour = GlobalTM.Hour + 1
'  GlobalTM.Mon  = GlobalTM.Mon  + 1
'  GlobalTM.MDay = GlobalTM.MDay + 1
'  GlobalTM.WDay = GlobalTM.WDay + 1
'  GlobalTM.Year = GlobalTM.Year + 1900
'End SUB
'
'Function Date$ () As String
'  Dim CurTime As Integer, Date AS TM
'  Print Time(AddressOf(GlobalTime))
'  Print AscTime(LocalTime(AddressOf(GlobalTime)))
'  FillGobalTM(LocalTime(AddressOf(GlobalTime)), AddressOf(GlobalTM))
'  Result = Str$(GlobalTM.Mon) + " " + Str$(GlobalTM.MDay) + " " + Str$(GlobalTM.Year)
'End Function
''

$End If