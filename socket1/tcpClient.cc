#include "tcpClient.hpp"

static void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "server_ip server_port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    int port_ = atoi(argv[2]);
    std::string ip_ = argv[1];
    Client *cp = new Client(port_, ip_);
    cp->InitClient();
    cp->Connect();
    cp->Run();
    delete cp;
    return 0;
}
