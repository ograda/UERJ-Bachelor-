#
# tcp - concorrente
#

CC = gcc
LIBS =  -lpthread -lnsl

todos:	Client Server

tcpServer: Server.c
	$(CC) -o Server Server.c $(LIBS)

tcpClient: tcpClient.c
	$(CC) -o Client Client.c $(LIBS)


