#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

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

class Server;

typedef struct {
    Server *sp;
    int sock;
}info_t;

class Server {
private:
    int listen_sock;
    int port;
    //std::string ip; //delete
public:
    Server(const int &port_):port(port_),
    listen_sock(-1)
    {}

    void InitServer()
    {
        //创建套接字
        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        
        //监听套接字
        if (listen_sock < 0)
        {
            std::cerr << "socket error" << std::endl;
            exit(2);
        }
        int opt = 1;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in local;
        bzero(&local, sizeof(local)); 

        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind error" << std::endl;
            exit(3);
        }

        if (listen(listen_sock, 5) < 0)
        {
            std::cerr << "listen error" << std::endl;
            exit(4);
        }

        //避免产生僵尸进程
        signal(SIGCHLD, SIG_IGN);
    }

    void Service(int sock)
    {
        char buf[1024];
        while(1) {
            ssize_t s = read(sock, buf, sizeof(buf)-1);
            if (s > 0)
            {
                buf[s] = '\0';
                std::cout << "client#" << buf << std::endl;
                write(sock, buf, strlen(buf));
            } else if (s == 0) {
                std::cout << "client quit!" << std::endl;
                break;
            } else {
                std::cerr << "read error" << std::endl;
                break;
            }
        }
    }

    static void *ThreadRun(void *arg)
    {
        pthread_detach(pthread_self());
        info_t *info = (info_t *)arg;
        Server *sp = info->sp;
        int sock = info->sock;
        delete info;
        sp->Service(sock);
        close(sock);
    }

    void Run()
    {
        struct sockaddr_in peer;
        for(;;)
        {
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if (sock < 0)
            {
                std::cerr << "accept error!" << std::endl;
                continue;
            }
            std::cout << "get a new client" << inet_ntoa(peer.sin_addr) << ":" << ntohs(peer.sin_port) << std::endl;
            pthread_t tid;
            info_t *info = new info_t;
            info->sock = sock;
            info->sp = this;
            pthread_create(&tid, NULL, ThreadRun, (void *)info);
            //pid_t id = fork();
            //if (id < 0)
            //{
            //  std::cout << "fork error" << std::endl;
            //} else if (id == 0)
            //{
            //  close(listen_sock);
            //  Service(sock);
            //  close(sock);
            //  exit(0);
            //}
        }
    }

    ~Server()
    {
        close(listen_sock);
    }
};

#endif
