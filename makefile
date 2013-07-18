all:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O2

run:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O2
	specialmoves.exe

test:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O2
	GetTiming.exe specialmoves.exe

compile:
	mingw32-gcc -o specialmoves.exe specialmoves.c -O2
