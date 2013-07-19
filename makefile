all:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O4

run:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O4
	specialmoves.exe

test:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O4
	GetTiming.exe specialmoves.exe

compile:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O4
