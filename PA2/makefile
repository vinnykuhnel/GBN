all: client server

packet.o: packet.cpp
	g++ -c packet.cpp

client: client.o packet.o
	g++ client.cpp packet.cpp -o client
	
server: server.o packet.o
	g++ server.cpp packet.cpp -o server	
	
clean:
	\rm *.o client server *.log
