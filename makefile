all:
	mingw32-gcc -o specialmoves.exe specialmoves.c

run:
	mingw32-gcc -o specialmoves.exe specialmoves.c
	specialmoves.exe

test:
	mingw32-gcc -o specialmoves.exe specialmoves.c
	GetTiming.exe specialmoves.exe

compile:
	mingw32-gcc -o specialmoves.exe specialmoves.c
