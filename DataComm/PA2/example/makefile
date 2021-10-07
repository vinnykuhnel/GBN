all: client server


client: exampleClient.o
	g++ exampleClient.cpp packet.cpp -o client
	
server: exampleServer.o
	g++ exampleServer.cpp packet.cpp -o server	
	
clean:
	\rm *.o client server
