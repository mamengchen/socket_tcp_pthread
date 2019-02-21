#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

class Client {
private:
    int sock;
    std::string peer_ip;
    int peer_port;
public:
    Client(const int &port_, const std::string &ip_)
        :peer_port(port_),
        peer_ip(ip_)
    {}

    void InitClient()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            std::cout << "socket error" << std::endl;
            exit(2);
        }
    }

    void Connect()
    {
        struct sockaddr_in peer;
        bzero(&peer, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(peer_port);
        peer.sin_addr.s_addr = inet_addr(peer_ip.c_str());

        if (connect(sock, (struct sockaddr*)&peer, sizeof(peer)) == 0)
        {
            std::cout << "connect success...." << std::endl;
        } else {
            std::cerr << "connect failed ..." << std::endl;
            exit(3);
        }
    }

    void Run()
    {
        std::string message;
        char buf[1024];
        while(1)
        {
            std::cout << "Please Enter> ";
            std::cin >> message;
            write(sock, message.c_str(), message.size());
            read(sock, buf, sizeof(buf)-1);
            std::cout << "server echo# " << buf << std::endl;
        }
    }

    ~Client()
    {
        close(sock);
    }
};

#endif
