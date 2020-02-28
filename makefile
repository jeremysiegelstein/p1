all: server client

server:	udp_server.o
	g++ udp_server.o -o server

udp_server.o: udp_server.cpp
	g++ -Wall -Wextra -pedantic -std=c++17 -c udp_server.cpp

client:	udp_client.o
    g++ udp_client.o -o client

udp_client.o: udp_client.cpp
    g++ -Wall -Wextra -pedantic -std=c++17 -c udp_client.cpp
clean:
	-rm -f *.o server
    -rm -f *.o client
