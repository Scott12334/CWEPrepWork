TCPEchoServer: TCPEchoServer.o DieWithMessage.o TCPServerUtility.o
	gcc TCPEchoServer.o DieWithMessage.o TCPServerUtility.o -o TCPEchoServer

TCPEchoServer.o:	TCPEchoServer.c Practical.h
	gcc -c TCPEchoServer.c

DieWithMessage.o:	DieWithMessage.c
	gcc -c DieWithMessage.c

TCPServerUtility.o:	TCPServerUtility.c Practical.h
	gcc -c TCPServerUtility.c

