#NOM : tcp-client
#OBJET : MakeFile client side
#HOWTO : make
#AUTEUR : Basile Routier 18/10/22

ClientEcho : Client
	@echo "./client is ready"

Client : tcpclient.o factoryMessage.o createSocket.o util.o
	@gcc -std=c99 -Wall -pedantic util.o createSocket.o factoryMessage.o tcpclient.o -o client

tcpclient.o : tcpclient.c
	@gcc -c -Wall tcpclient.c -o tcpclient.o

factoryMessage.o : factoryMessage.c
	@gcc -c -Wall factoryMessage.c -o factoryMessage.o

createSocket.o : createSocket.c
	@gcc -c -Wall createSocket.c -o createSocket.o
util.o : util.c
	@gcc -c -Wall util.c -o util.o


clean :
	@rm *.o

cleanall: clean
	@rm client