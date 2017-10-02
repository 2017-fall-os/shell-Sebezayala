all: myShell

CFLAGS = -g -O2

myShell: myShell.o mytoc.o utils.o shellFunctions.o
	cc -o myShell myShell.o mytoc.o utils.o shellFunctions.o
clean:
	rm -f  *.o myShell myShell.exe
