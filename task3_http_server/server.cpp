#include"server.h"
#include<iostream>
using namespace std;

class Socket
{
    private:
        int socketfd;
        struct sockaddr_in servaddr, cliaddr;
        void Bind();
        void Listen();
    public:
        int get_fd();
        void initilize();//bind and listen
        Socket();
        ~Socket();
};

int Socket::get_fd()
{
    return socketfd;
}

Socket::Socket()
{
    int listenfd;
    if((listenfd=socket(AF_INET, SOCK_STREAM, 0)<0))
    {
        std::cout<<"socket wrong"<<std::endl;
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    socketfd=listenfd;
    std::cout<<"socket created"<<std::endl;
}

Socket::~Socket()
{
    close(socketfd);
    std::cout<<"socket closed"<<std::endl;
}

void Socket::Bind()
{
    if(bind(socketfd,(struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        std::cout<<"bind wrong"<<std::endl;
    }
}

void Socket::Listen()
{
    if(listen(socketfd, BACKLOG)<0)
    {
        std::cout<<"listen wrong"<<std::endl;
    }
}

void Socket::initilize()
{
    Bind();
    Listen();
    std::cout<<"socket bind at: "<<servaddr.sin_addr.s_addr<<"and keep listening!"<<std::endl;
}

int main()
{
    class Socket my_socket;
    my_socket.initilize();
    std::cout<<"socket init"<<std::endl;
    return 0;
}