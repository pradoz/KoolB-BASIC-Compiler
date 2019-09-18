' String.inc - String functions 

$IfNDef Stringinc
$Define Stringinc

'$Include "System.inc"
$Include "Type.inc"

Declare Function Len Lib "Kernel32.dll" Alias "lstrlenA" Call Std (S As String) As Integer
Declare Function lstrcpy Lib "Kernel32.dll" Alias "lstrcpyA" (byref S1 As String, byval S2 As String) As String

Function Space$(Size As Integer) As String
  Dim Counter As Integer
  Counter = 1
  Result = ""
  While Counter <= Size
    Result = Result + " "
    Counter = Counter + 1
  WEnd
End Function

Function Mid$(S As String, Start As Integer, Length As Integer) As String
  If Start < 1 Then
    Start = 1
  End If
  If Start + Length > LEN(S) Then
    Length = LEN(S) - Start + 1
  End If
  $Asm
    extern lstrlenA
    extern lstrcpyA
    extern lstrcpynA
    MOV EBX,dword[EBP+8]   ;EBX = S
    MOV ESI,dword[EBP+12]  ;ESI = Start
    MOV EDI,dword[EBP+16]  ;EDI = Length
    DEC ESI                ;Convert to zero-based 
    INC EDI
    ADD EBX,ESI            ;Move Ptr to Start
    ADD ESI,EDI
    INC ESI
    stdcall HeapAlloc,dword[HandleToHeap],8,ESI
    DEC ESI
    stdcall lstrcpynA,EAX,EBX,EDI
    MOV dword[EBP-4],EAX
  $End Asm
End Function

Function Asc(S As String) As Integer
  $Asm
    MOV EAX,dword[EBP+8]
    MOVSX EAX,byte[EAX]
    MOV dword[EBP-4],EAX
  $End Asm
End Function

Function Chr$(ASCII As Integer) As String
  $Asm
    stdcall HeapAlloc,dword[HandleToHeap],8,2
    MOV EBX,EAX
    MOV AH,byte[EBP+8]
    MOV byte[EBX],AH
    MOV dword[EBP-4],EBX
  $End ASM
End Function

Function Delete$(S As String, Start As Integer, Length As Integer) As String
  Result = Mid$(S, 1, Start - 1) + Mid$(S, Start + Length, Len(S))
End Function

Function Insert$(Insert As String, Source As String, Index As Integer) As String
  Result = Mid$(Source, 1, Index - 1) + Insert + Mid$(Source, Index, Len(Source))
End Function

Function LCase$(S As String) As String
  Result = ""
  Index& = 1
  Length& = Len(S) + 1
  While Index& < Length&
    Result = Result + Chr$(ToLower(Asc(Mid$(S, Index&, 1))))
    Index& = Index& + 1
  WEnd
End Function

Function UCase$(S As String) As String
  Result = ""
  Index& = 1
  Length& = Len(S) + 1
  While Index& < Length&
    Result = Result + Chr$(ToUpper(Asc(Mid$(S, Index&, 1))))
    Index& = Index& + 1
  WEnd
End Function

Function Instr(Start As Integer, S As String, Find As String) As Integer
  Dim Found As Integer, Length As Integer, SearchableLength As Integer
  If Start < 1 Then
    Start = 1
  End If
  Length = Len(Find)
  Found = 0
  SearchableLength = Len(S) 
  While Start <= SearchableLength And Found = 0
    If Mid$(S, Start, Length) = Find Then
      Found = 1
      Result = Start
    End If
    Start = Start + 1
  WEnd
  If Found = 0 Then
    Result = 0
  End If
End Function 

Function Left$(S As String, N As Integer) As String
  Result = Mid$(S, 1, N)
End Function

Function LTrim$(S As String) As String
  Dim Length As Integer, Index As Integer, Found As Integer
  Index = 1
  Length = Len(S)
  Found = 0
  While Index < Length And Found = 0
    If Mid$(S, Index, 1) <> " " Then
      Found = 1
    End If
    Index = Index + 1
  WEnd
    Index = Index - 1   'Counter act last increment
  Result = Mid$(S, Index, Len(S) - Index + 1)
End Function

Function Replace$(Source As String, Replace As String, Index As Integer) As String
  Result = Mid$(Source, 1, Index - 1) + Replace + Mid$(Source, Index + Len(Replace), Len(Source))
End Function

Function ReplaceSubStr$(Source As String, Replace As String, Replacement As String) As String
  Dim Index As Integer
  Index = InStr(1, Source, Replace) 
  While Index <> 0
    Source = Mid$(Source, 1, Index - 1) + Replacement + Mid$(Source, Index + Len(Replacement) + 1, Len(Source))
    Index = Index + Len(Replacement)
    Index = InStr(Index + 1, Source, Replace) 
  WEnd
  Result = Source
End Function

Function Reverse$(S As String) As String
  Dim Length As Integer, Index As Integer
  Index = 1
  Length = Len(S)
  While Index <= Length 
    Result = Mid$(S, Index, 1) + Result
    Index = Index + 1
  WEnd
End Function

Function Right$ (S As String, N As Integer) As String
  Result = Mid$(S, N, Len(S))
End Function

Function RInstr(Start As Integer, S As String, Find As String) As Integer
  Dim Found As Integer, Length As Integer, SearchableLength As Integer
  If Start = 0 Then
    Start = Len(S) - Len(Find)
  End If
  Length = Len(Find)
  Found = 0
  While Start >= 1 And Found = 0
    If Mid$(S, Start, Length) = Find Then
      Found = 1
      Result = Start' + Len(Find)
    End If
    Start = Start - 1
  WEnd
  If Found = 0 Then
    Result = 0
  End If
End Function 

Function RTrim$(S As String) As String
  Dim Index As Integer, Found As Integer
  Index = Len(S)
  Found = 0
  While Index >= 1 And Found = 0
    If Mid$(S, Index, 1) <> " " Then
      Found = 1
    End If
    Index = Index - 1
  WEnd
    Index = Index + 1   'Counter act last increment
  Result = Mid$(S, 1, Index)
End Function

Function String$(Number As Integer, S As String) As String
  Dim Index As Integer
  Index = 1
  While Index <= Number
    Result = Result + Mid$(S, 1, 1)
    Index = Index + 1
  WEnd
End Function

Function Tally(Search As String, Match As String) As Integer
  Dim Position As Integer, AtEnd As Integer
  Position = 1
  AtEnd = 0
  While AtEnd = 0
    Position = Instr(Position, Search, Match)
    If Position = 0 Then
      AtEnd = 1
    End If
    Position = Position + 1
    Result = Result + 1
  WEnd
  Result = Result
End Function

$End If