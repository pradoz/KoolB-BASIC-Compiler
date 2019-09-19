cls & g++ -g main.cpp -o test
TIMEOUT /T 1 /NOBREAK
rem test "simple.bas"
:: Asm\GoRC /r /ni /nw "simple.rc"
:: Asm\GoLink /ni /nw "simple.obj" "simple.res" kernel32.dll user32.dll gdi32.dll msvcrt.dll