#NOM : tcp-server
#OBJET : MakeFile server side
#HOWTO : make
#AUTEUR : Basile Routier 18/10/22

ServerEcho : Server
	@echo "sudo ./server is ready"

Server : tcpserver.o factoryClient.o createSocket.o util.o
	@gcc -std=c99 -Wall -pedantic util.o createSocket.o factoryClient.o tcpserver.o -o server

tcpserver.o : tcpserver.c
	@gcc -c -Wall tcpserver.c -o tcpserver.o
factoryClient.o : factoryClient.c
	@gcc -c -Wall factoryClient.c -o factoryClient.o
createSocket.o : createSocket.c
	@gcc -c -Wall createSocket.c -o createSocket.o
util.o : util.c
	@gcc -c -Wall util.c -o util.o

clean :
	@rm *.o

cleanall: clean
	@rm server