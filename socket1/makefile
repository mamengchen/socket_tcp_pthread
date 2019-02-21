.PHONY:all
all:tcpClient tcpServer

tcpClient:tcpClient.cc
	g++ -o $@ $^ -static
tcpServer:tcpServer.cc
	g++ -o $@ $^ -static -lpthread

.PHONY:clean
clean:
	rm tcpClient tcpServer
