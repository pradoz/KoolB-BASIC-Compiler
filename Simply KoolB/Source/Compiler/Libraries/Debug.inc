' 
' --- Debug.inc --- 
' 
' 

$Include "Win32API.inc" 
$Include "Libraries/String.inc" 

$Const FORMAT_MESSAGE_ALLOCATE_BUFFER = 256 
$Const FORMAT_MESSAGE_IGNORE_INSERTS = 512 
$Const FORMAT_MESSAGE_FROM_STRING = 1024 
$Const FORMAT_MESSAGE_FROM_HMODULE = 2048 
$Const FORMAT_MESSAGE_FROM_SYSTEM = 4096 
$Const FORMAT_MESSAGE_ARGUMENT_ARRAY = 8192 
$Const FORMAT_MESSAGE_MAX_WIDTH_MASK = 255 

' Use this for LANG_NEUTRAL+SUBLANG_NEUTRAL. 
$Const LANG_NEUTRAL = 0 

' This included for test. 
Declare Sub SetLastError Lib "kernel32" Alias "SetLastError" _ 
   (dwErrCode As Integer) 

' The ByRef here is necessary. 
Declare Function LTOA Lib "msvcrt.dll" Alias "_ltoa" _ 
   Call C _ 
   (number As Integer, _ 
   ByRef lpString As String, _ 
   radix As Integer) _ 
   As Integer 

Sub ShowInt _ 
   (caption As String, _ 
   i As Integer, _ 
   radix As Integer) 
    
   Dim iString As String 
   iString = Space$(40) 
   LTOA(i,iString,radix) 
    
   MessageBox(0,AddressOf(iString),AddressOf(caption),0)    
End Sub 

Sub ShowLastError() 

   Dim lpMsgBuf As Integer 
   Dim dwFlags As Integer 
   Dim dwMessageId As Integer    
   Dim idString As String 
   Dim caption as String 

   dwMessageId = GetLastError() 
   idString = Space$(40) 
   LTOA(dwMessageId,idString,10) 
   caption = "System error code "+idString 
    
   dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER + _ 
      FORMAT_MESSAGE_FROM_SYSTEM + _ 
      FORMAT_MESSAGE_IGNORE_INSERTS    

   FormatMessage _ 
      (dwFlags, _ 
      0, _ 
      dwMessageId, _ 
      LANG_NEUTRAL, _ 
      AddressOf(lpMsgBuf), _ 
      0, _ 
      0)    
       
   MessageBox(0,lpMsgBuf,AddressOf(caption),0) 

   LocalFree(lpMsgBuf) 
End Sub 
