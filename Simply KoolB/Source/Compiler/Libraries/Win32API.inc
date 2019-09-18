' 
' --- Win32API.inc --- 
' 
' 
' Because some API functions require that pointers to 
' strings be handled as Integers, and because KoolB does 
' not provide (AFAIK) any way to disable parameter type 
' checking, all (*) lpsz parameters are declared as 
' Integers. 
' 
' * The single exception is the MsgBox function, which 
' takes string parameters for convenience. 

$IfNDef Win32APIinc 
$Define Win32APIinc 

' --- Window --- 

'$Const CS_VREDRAW = 1 
'$Const CS_HREDRAW = 2 
'$Const CS_CLASSDC = 64 
' 
'$Const COLOR_WINDOW = 5 
' 
'$Const IDC_ARROW = 32512 
' 
'$Const IDI_APPLICATION = 32512 
'$Const IDI_WINLOGO = 32517 
' 
' *** WS_POPUP will prevent the window from being 
' *** displayed even though RegisterClassEx and 
' *** CreateWindowEx will return success. 
'$Const WS_POPUP = 2147483648 
' 
' -- Begin WS_DEFAULT -- 
'$Const WS_VISIBLE = 268435456 
'$Const WS_CAPTION = 12582912 
'$Const WS_SIZEBOX = 262144 
'$Const WS_SYSMENU = 524288 
'$Const WS_MINIMIZEBOX = 131072 
'$Const WS_MAXIMIZEBOX = 65536 
' -- End WS_DEFAULT -- 
'$Const WS_DEFAULT = 282001408 
' 
'$Const CW_USEDEFAULT = 2147483648 

Type TWNDCLASSEX 
   cbSize As Integer 
   style As Integer 
   lpfnWndProc As Integer 
   cbClsExtra As Integer 
   cbWndExtra As Integer 
   hInstance As Integer 
   hIcon As Integer 
   hCursor As Integer 
   hbrBackground As Integer 
   lpszMenuName As Integer 
   lpszClassName As Integer 
   hIconSm As Integer 
End Type 

Declare Function RegisterClassEx Lib "user32" Alias "RegisterClassExA" _ 
   (ByRef wcex As TWNDCLASSEX)_ 
   As Integer 

' This declaration assumes that lpModuleName will be set 
' to zero to get the handle to the file used to create 
' the calling process. 
Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" _ 
   (lpModuleName As Integer) _ 
   As Integer 

' This declaration assumes the use of one of the predefined 
' cursors. Set hInstance to zero and cursorID to one of the 
' IDC_ constants. 
Declare Function LoadCursor Lib "user32" Alias "LoadCursorA" _ 
   (hInstance As Integer, _ 
   cursorID As Integer) _ 
   As Integer 

' This declaration assumes the use of one of the standard 
' icons. Set hInstance to zero and iconID to one of the 
' IDI_ constants. 
Declare Function LoadIcon Lib "user32" Alias "LoadIconA" _ 
   (hInstance As Integer, _ 
   iconID As Integer) _ 
   As Integer 

Declare Function CreateWindowEx Lib "user32" Alias "CreateWindowExA" _ 
   (dwExStyle As Integer, _ 
   lpClassName As Integer, _ 
   lpWindowName As Integer, _ 
   dwStyle As Integer, _ 
   x As Integer, _ 
   y As Integer, _ 
   nWidth As Integer, _ 
   nHeight As Integer, _ 
   hWndParent As Integer, _ 
   hMenu As Integer, _ 
   hInstance As Integer, _ 
   lpParam As Integer) _ 
   As Integer 

Declare Function UnregisterClass Lib "user32" Alias "UnregisterClassA" _ 
   (lpClassName As Integer, _ 
   hInstance As Integer) _ 
   As Integer 

' --- Message --- 

'$Const WM_CREATE = 1 
'$Const WM_DESTROY = 2 
'$Const WM_MOVE = 3 
'$Const WM_SIZE = 5 
'$Const WM_PAINT = 15 
'$Const WM_QUIT = 18 

Type TMSG 
   hWnd As Integer 
   message As Integer 
   wParam As Integer 
   lParam As Integer 
   time As Integer 
   x As Integer 
   y As Integer    
End Type 

' *** The following three declarations specify an integer 
' *** pointer to the message structure because I could 
' *** not get the functions to work correctly with the 
' *** default parameter passing for structures. 
Declare Function GetMessage Lib "user32" Alias "GetMessageA" _ 
   (lpMsg As Integer, _ 
   hWnd As Integer, _ 
   wMsgFilterMin As Integer, _ 
   wMsgFilterMax As Integer) _ 
   As Integer 

Declare Function TranslateMessage Lib "user32" Alias "TranslateMessage" _ 
   (lpMsg As Integer) _ 
   As Integer 

Declare Function DispatchMessage Lib "user32" Alias "DispatchMessageA" _ 
   (lpMsg As Integer) _ 
   As Integer 

Declare Function SendMessage Lib "user32" Alias "SendMessageA" _ 
   (hWnd As Integer, _ 
   wMsg As Integer, _ 
   wParam As Integer, _ 
   lParam As Integer) _ 
   As Integer 

' --- WindowProc --- 

Type TPAINTSTRUCT 
   hdc As Integer 
   fErase As Integer 
   ' -- Begin rcPaint As RECT -- 
   left As Integer 
   top As Integer 
   right As Integer 
   bottom As Integer 
   ' -- End rcPaint As RECT -- 
   fRestore As Integer 
   fIncUpdate As Integer 
   ' -- Begin BYTE rgbReserved[32] -- 
   rgbReserved1 As Integer 
   rgbReserved2 As Integer 
   rgbReserved3 As Integer 
   rgbReserved4 As Integer 
   rgbReserved5 As Integer 
   rgbReserved6 As Integer 
   rgbReserved7 As Integer 
   rgbReserved8 As Integer 
   ' -- End BYTE rgbReserved[32] -- 
End Type 

Declare Function DefWindowProc Lib "user32" Alias "DefWindowProcA" _ 
   (hWnd As Integer, _ 
   wMsg As Integer, _ 
   wParam As Integer, _ 
   lParam As Integer) _ 
   As Integer 

Declare Function BeginPaint Lib "user32" Alias "BeginPaint" _ 
   (hWnd As Integer, _ 
   ByRef lpPaint As TPAINTSTRUCT) _ 
   As Integer 

Declare Function EndPaint Lib "user32" Alias "EndPaint" _ 
   (hwnd As Integer, _ 
   ByRef lpPaint As TPAINTSTRUCT) _ 
   As Integer 

Declare Function TextOut Lib "gdi32" Alias "TextOutA" _ 
   (hdc As Integer, _ 
   xStart As Integer, _ 
   yStart As Integer, _ 
   lpString As Integer, _ 
   nCount As Integer) _ 
   As Integer 

Declare Sub PostQuitMessage Lib "user32" Alias "PostQuitMessage" _ 
   (nExitCode As Integer) 

' --- Subclass --- 

'$Const GWL_WNDPROC = -4 

Declare Function GetWindowLong Lib "user32" Alias "GetWindowIntegerA" _ 
   (hwnd As Integer, _ 
   nIndex As Integer) _ 
   As Integer 

Declare Function SetWindowLong Lib "user32" Alias "SetWindowIntegerA" _ 
   (hwnd As Integer, _ 
   nIndex As Integer, _ 
   dwNewLong As Integer) _ 
   As Integer 

Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" _ 
   (lpPrevWndFunc As Integer, _ 
   hWnd As Integer, _ 
   wMsg As Integer, _ 
   wParam As Integer, _ 
   lParam As Integer) _ 
   As Integer 

' --- Misc --- 

Declare Function GetActiveWindow Lib "user32" Alias "GetActiveWindow" _ 
   () _ 
   As Integer 

Declare Sub ExitProcess Lib "kernel32" Alias "ExitProcess" _ 
   (uExitCode As Integer) 
    
Declare Function MsgBox Lib "user32" Alias "MessageBoxA" _ 
   (hWnd As Integer, _ 
   lpText As String, _ 
   caption As String, _ 
   wType As Integer) _ 
   As Integer 
    
Declare Function MessageBox Lib "user32" Alias "MessageBoxA" _ 
   (hWnd As Integer, _ 
   lpText As Integer, _ 
   caption As Integer, _ 
   wType As Integer) _ 
   As Integer 
    
Declare Function GetLastError Lib "kernel32" Alias "GetLastError" _ 
   () _ 
   As Integer 

'$Const FORMAT_MESSAGE_ALLOCATE_BUFFER = 256 
'$Const FORMAT_MESSAGE_IGNORE_INSERTS = 512 
'$Const FORMAT_MESSAGE_FROM_STRING = 1024 
'$Const FORMAT_MESSAGE_FROM_HMODULE = 2048 
'$Const FORMAT_MESSAGE_FROM_SYSTEM = 4096 
'$Const FORMAT_MESSAGE_ARGUMENT_ARRAY = 8192 
'$Const FORMAT_MESSAGE_MAX_WIDTH_MASK = 255 

' Use this for LANG_NEUTRAL+SUBLANG_NEUTRAL. 
' $Const LANG_NEUTRAL = 0 

Declare Function FormatMessage Lib "kernel32" Alias "FormatMessageA" _ 
   (ByVal dwFlags As Integer, _ 
   lpSource As Integer, _ 
   ByVal dwMessageId As Integer, _ 
   ByVal dwLanguageId As Integer, _ 
   ByVal lpBuffer As Integer, _ 
   ByVal nSize As Integer, _ 
   Arguments As Integer) _ 
   As Integer 

Declare Function LocalFree Lib "kernel32" Alias "LocalFree" _ 
   (hMem As Integer) _ 
   As Integer 
    
Sub Return(i As Integer) 
   $Asm 
      mov   eax,[ebp+8] 
   $End Asm    
End Sub    

$End If 