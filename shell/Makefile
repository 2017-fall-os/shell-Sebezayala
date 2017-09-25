all: myShell

CFLAGS = -g -O2

myShell: myShell.o mytoc.o utils.o
	cc -o myShell myShell.o mytoc.o utils.o
clean:
	rm -f  *.o myShell myShell.exe
