' Windows.inc - Windows API stuff

'$IfNDef Windowsinc
'$Define Windowsinc


'-- Standard Icon IDs
$CONST IDI_APPLICATION = 32512
$CONST IDI_HAND = 32513
$CONST IDI_QUESTION = 32514
$CONST IDI_EXCLAMATION = 32515
$CONST IDI_ASTERISK = 32516
$CONST IDI_WINLOGO = 32517

'-- Class Styles
$CONST CS_VREDRAW = 1
$CONST CS_HREDRAW = 2
$CONST CS_KEYCVTWINDOW = 4
$CONST CS_DBLCLKS = 8
$Const CS_CLASSDC = 64
$Const CS_PARENTDC = 128

$Const CW_USEDEFAULT = 2147483648
$Const HWND_DESKTOP = 0

'-- Window styles
$CONST WS_CAPTION = 12582912
$CONST WS_CHILD = 1073741824
$CONST WS_VISIBLE = 268435456
$CONST WS_BORDER = 8388608
$CONST WS_SYSMENU = 524288
$CONST WS_THICKFRAME = 262144
$CONST WS_SIZEBOX = 262144


$CONST WS_MINIMIZEBOX = 131072
$CONST WS_MAXIMIZEBOX = 65536


$CONST WS_EX_MDICHILD = 64
$CONST WS_EX_TOOLWINDOW = 128
$CONST WS_EX_WINDOWEDGE = 256
$CONST WS_EX_CLIENTEDGE = 512
$CONST WS_EX_CONTEXTHELP = 1024

$CONST WS_EX_LEFTSCROLLBAR = 16384
$CONST WS_EX_RIGHTSCROLLBAR = 0

'-- Button styles
$CONST BS_PUSHBUTTON = 0
$CONST BS_DEFPUSHBUTTON = 1
$CONST BS_CHECKBOX = 2
$CONST BS_AUTOCHECKBOX = 3
$CONST BS_RADIOBUTTON = 4
$CONST BS_3STATE = 5
$CONST BS_AUTO3STATE = 6
$CONST BS_GROUPBOX = 7
$CONST BS_USERBUTTON = 8
$CONST BS_AUTORADIOBUTTON = 9
$CONST BS_OWNERDRAW = 11
$CONST BS_LEFTTEXT = 32
$CONST BS_TEXT = 0
$CONST BS_ICON = 64
$CONST BS_BITMAP = 128
$CONST BS_LEFT = 256
$CONST BS_RIGHT = 512
$CONST BSCENTER = 768
$CONST BSTOP = 1024
$CONST BSBOTTOM = 2024
$CONST BSVCENTER = 3072
$CONST BSPUSHLIKE = 4096
$CONST BSMULTILINE = 8192
$CONST BSNOTIFY = 16384
$CONST BSFLAT = 32768
$CONST BSRIGHTBUTTON = 32

$CONST WMDESTORY = 2

'-- System colors
$CONST clScrollBar = -2147483648
$CONST clBackGround = -2147483647
$CONST clActiveCaption = -2147483646
$CONST clInActiveCaption = -2147483645
$CONST clMenu = -2147483644
$CONST clWindow = -2147483643
$CONST clWindowFrame = -2147483642
$CONST clMenuText = -2147483641
$CONST clWindowText = -2147483640
$CONST clCaptionText = -2147483639
$CONST clActiveBorder = -2147483638
$CONST clInActiveBorder = -2147483637
$CONST clAppWorkSpace = -2147483636
$CONST clHilight = -2147483635
$CONST clHilightText = -2147483634
$CONST clBtnFace = -2147483633
$CONST clBtnShadow = -2147483632
$CONST clGrayText = -2147483631
$CONST clBtnText = -2147483630
$CONST clInActiveCaptionText = -2147483629
$CONST clBtnHighlight = -2147483628
$CONST cl3DDkShadow = -2147483627
$CONST cl3DLight = -2147483626
$CONST clInfoText = -2147483625
$CONST clInfoBk3DDkShadow = -2147483624
$CONST clNone = 536870911
$CONST clDefault = 536870912


'---------------------------------
' ADDED FOR WINDOW CREATION TEST
'----------------------------------
$Const WSEXSTATICEDGE = 131072 '&H20000 ORIGINAL NAME WS_EX_STATICEDGE (Need to support _ )
$Const WSEXTRANSPARENT = 32 '&H20&    BE GOOD IF THE COMPILER SUPPORT &H TYPE "HEX"
$Const CWUSEDEFAULT = 2147483648 '&H80000000
$Const SWNORMAL = 1

' Window styles
$Const WS_OVERLAPPED = 0
$Const WS_POPUP = 2147483648
$Const WS_MINIMIZE = 536870912
$Const WS_DISABLED = 134217728
$Const WS_CLIPSIBLINGS = 67108864
$Const WS_CLIPCHILDREN = 33554432
$Const WS_MAXIMIZE = 16777216
$Const WS_DLGFRAME = 4194304
$Const WS_VSCROLL = 2097152
$Const WS_HSCROLL = 1048576
$Const WS_GROUP = 131072
$Const WS_TABSTOP = 65536
$Const WS_TILED = 0
$Const WS_ICONIC = 536870912
$Const WS_OVERLAPPEDWINDOW = 13565952
$Const WS_TILEDWINDOW = 13565952
$Const WS_POPUPWINDOW = 2156396544
$Const WS_CHILDWINDOW = 1073741824

' ExWindowStyles
$Const WS_EX_DLGMODALFRAME = 1
$Const WS_EX_NOPARENTNOTIFY = 4
$Const WS_EX_TOPMOST = 8
$Const WS_EX_ACCEPTFILES = 16
$Const WS_EX_TRANSPARENT = 32

' Color constants
$Const COLOR_SCROLLBAR = 0
$Const COLOR_BACKGROUND = 1
$Const COLOR_ACTIVECAPTION = 2
$Const COLOR_INACTIVECAPTION = 3
$Const COLOR_MENU = 4
$Const COLOR_WINDOW = 5
$Const COLOR_WINDOWFRAME = 6
$Const COLOR_MENUTEXT = 7
$Const COLOR_WINDOWTEXT = 8
$Const COLOR_CAPTIONTEXT = 9
$Const COLOR_ACTIVEBORDER = 10
$Const COLOR_INACTIVEBORDER = 11
$Const COLOR_APPWORKSPACE = 12
$Const COLOR_HIGHLIGHT = 13
$Const COLOR_HIGHLIGHTTEXT = 14
$Const COLOR_BTNFACE = 15
$Const COLOR_BTNSHADOW = 16
$Const COLOR_GRAYTEXT = 17
$Const COLOR_BTNTEXT = 18
$Const COLOR_INACTIVECAPTIONTEXT = 19
$Const COLOR_BTNHIGHLIGHT = 20

' Window messages
$Const WM_NULL = 0
$Const WM_CREATE = 1
$Const WM_DESTROY = 2
$Const WM_MOVE = 3
$Const WM_SIZE = 5
$Const WM_CLOSE = 16

' ShowWindow commands
$Const SW_HIDE = 0
$Const SW_SHOWNORMAL = 1
$Const SW_NORMAL = 1
$Const SW_SHOWMINIMIZED = 2
$Const SW_SHOWMAXIMIZED = 3
$Const SW_MAXIMIZE = 3
$Const SW_SHOWNOACTIVATE = 4
$Const SW_SHOW = 5
$Const SW_MINIMIZE = 6
$Const SW_SHOWMINNOACTIVE = 7
$Const SW_SHOWNA = 8
$Const SW_RESTORE = 9
$Const SW_SHOWDEFAULT = 10
$Const SW_MAX = 10


' Standard ID's of cursors
$Const IDC_ARROW = 32512
$Const IDC_IBEAM = 32513
$Const IDC_WAIT = 32514
$Const IDC_CROSS = 32515
$Const IDC_UPARROW = 32516
$Const IDC_SIZE = 32640
$Const IDC_ICON = 32641
$Const IDC_SIZENWSE = 32642
$Const IDC_SIZENESW = 32643
$Const IDC_SIZEWE = 32644
$Const IDC_SIZENS = 32645
$Const IDC_SIZEALL = 32646
$Const IDC_NO = 32648
$Const IDC_APPSTARTING = 32650
$Const GWL_WNDPROC = -4


Type WNDCLASS
    style As Integer
    lpfnwndproc As Integer
    cbClsextra As Integer
    cbWndExtra2 As Integer
    hInstance As Integer
    hIcon As Integer
    hCursor As Integer
    hbrBackground As Integer
    lpszMenuName As String
    lpszClassName As String
End Type


Type CREATESTRUCT
    lpCreateParams As Integer
    hInstance As Integer
    hMenu As Integer
    hWndParent As Integer
    cy As Integer
    cx As Integer
    y As Integer
    x As Integer
    style As Integer
    lpszName As String
    lpszClass As String
    ExStyle As Integer
End Type

Type Msg
    hWnd As Integer
    message As Integer
    wParam As Integer
    lParam As Integer
    time As Integer
    pt As Integer 'HERE NEED TO BE (pt As POINTAPI)
End Type
'-------------------------------------------




Type RECT
        Left As Integer
        Top As Integer
        Right As Integer
        Bottom As Integer
End Type

Type RECTL
        Left As Integer
        Top As Integer
        Right As Integer
        Bottom As Integer
End Type

Type POINTAPI
        x As Integer
        y As Integer
End Type

Type POINTL
        x As Integer
        y As Integer
End Type

Type Size
        cx As Integer
        cy As Integer
End Type

Type POINTS
        x  As Integer
        y  As Integer
End Type

Type STARTUPINFO
        cb As Integer
        lpReserved As String
        lpDesktop As String
        lpTitle As String
        dwX As Integer
        dwY As Integer
        dwXSize As Integer
        dwYSize As Integer
        dwXCountChars As Integer
        dwYCountChars As Integer
        dwFillAttribute As Integer
        dwFlags As Integer
        wShowWindow As Integer
        cbReserved2 As Integer
        lpReserved2 As Integer
        hStdInput As Integer
        hStdOutput As Integer
        hStdError As Integer
End Type

Type WNDCLASSEX
    cbSize As Integer
    style As Integer
    lpfnWndProc As Integer
    cbClsExtra As Integer
    cbWndExtra As Integer
    hInstance As Integer
    hIcon As Integer
    hCursor As Integer
    hbrBackground As Integer
    lpszMenuName As String
    lpszClAssName As String
    hIconSm As Integer
End Type

TYPE TMSG
    hwnd As Integer
    message As Integer
    wParam As Integer
    lParam As Integer
    time As Integer
    x As Integer
    y As Integer
END TYPE

'---------------------------------
' ADDED FOR WINDOW CREATION TEST
'----------------------------------

DECLARE FUNCTION UnregisterClass LIB "USER32" Alias "UnregisterClassA" _
                 (lpClassName As String, hInstance As Integer) As Integer
DECLARE FUNCTION GetWindowLong LIB "USER32" Alias "GetWindowLongA" (hWnd As Integer, nIndex As Integer) As Integer
DECLARE FUNCTION SetWindowLong LIB "USER32" Alias "SetWindowLongA" (hWnd As Integer, BnIndex As Integer, dwNewLong As Integer) As Integer
DECLARE FUNCTION CallWindowProc LIB "USER32" Alias "CallWindowProcA" (lpPrevWndFunc As Integer, hWnd As Integer, Mesg As Integer, wParam As Integer, lParam As Integer) As Integer
DECLARE FUNCTION RegisterClass Lib "USER32" Alias "RegisterClassA" (ByRef Class As WNDCLASS) As Integer
'----------------------------------
Declare Function CreateMenu LIB "user32.dll" Alias "CreateMenu" () As Integer

DECLARE FUNCTION RegisterClAssEx LIB "USER32" Alias "RegisterClassExA" _
                 (ByRef wndcls As WNDCLASSEX) As Integer

DECLARE FUNCTION GetMessage LIB "USER32" Alias "GetMessageA" _
                 (ByRef lpMsg As TMSG, hWnd As Integer, wMsgFilterMin As Integer, _
                  wMsgFilterMax As Integer) As Integer

DECLARE FUNCTION LoadCursor LIB "USER32" Alias "LoadCursorA" _
                 (hInst As Integer, lpCursorName As Integer) As Integer

DECLARE FUNCTION LoadIcon LIB "USER32" Alias "LoadIconA" _
                 (hInst As Integer, lpIconName As Integer) As Integer

DECLARE FUNCTION CreateWindowEx LIB "USER32" Alias "CreateWindowExA" _
                 (ExStyle As Integer, ClAssName As String, WindowName As String, _
                  Style As Integer, X  As Integer, Y As Integer, _
                  Width As Integer, Height As Integer, WndParent As Integer, _
                  hMenu As Integer, hInstance As Integer, Param As Integer) As Integer

DECLARE FUNCTION UpdateWindow LIB "USER32" Alias "UpdateWindow" _
                 (hWnd As Integer) As Integer

DECLARE FUNCTION ShowWindow LIB "USER32" Alias "ShowWindow" _
                 (hWnd As Integer, Show As Integer) As Integer

DECLARE FUNCTION DefWindowProc LIB "USER32" Alias "DefWindowProcA" _
                 (hWnd As Integer, Mesg As Integer, wParam As Integer, lParam As Integer) As Integer

DECLARE FUNCTION PostQuitMessage LIB "USER32" Alias "PostQuitMessage" _
                 (nExitCode As Integer) As Integer

DECLARE FUNCTION TranslateMessage LIB "USER32" Alias "TranslateMessage" _
                 (ByRef lpMsg As TMSG) As Integer

DECLARE FUNCTION DispatchMessage LIB "USER32" Alias "DispatchMessageA" _
                 (ByRef lpMsg As TMSG) As Integer

Declare Function GetModuleHandle Lib "Kernel32.dll" Alias "GetModuleHandleA" (N As Integer) As Integer
Declare Function GetLastErr LIB "kernel.dll" Alias "GetLastError" (N As Integer) As Integer

'MessageBox Api Declaration
$Const MB_DEFBUTTON1 = 0
$Const MB_DEFBUTTON2 = 256
$Const MB_DEFBUTTON3 = 512
$Const MB_ICONASTERISK = 64
$Const MB_ICONEXCLAMATION = 48
$Const MB_ICONHAND = 16
$Const MB_ICONINFORMATION = 64
$Const MB_ICONQUESTION = 32
$Const MB_ICONSTOP = 16
$Const MB_OK = 0
$Const VBOKONLY = 0 'VB COMPATIBILITY
$Const MB_OKCANCEL = 1
$Const MB_YESNO = 4
$Const MB_YESNOCANCEL = 3
$Const MB_ABORTRETRYIGNORE = 2
$Const MB_RETRYCANCEL = 5




'----- Lybrary Api Functions
Declare Function GetProcAddress Lib "kernel32" Alias "GetProcAddress" (hModule As Integer, lpProcName As String) As Integer



'$End If


