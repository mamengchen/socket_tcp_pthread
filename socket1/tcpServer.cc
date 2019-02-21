#include "tcpServer.hpp"

static void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    int port_ = atoi(argv[1]);
    Server *sp = new Server(port_);
    sp->InitServer();
    sp->Run();
    delete sp;
    return 0;
}
