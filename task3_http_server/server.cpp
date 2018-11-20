#include"server.h"
#include<iostream>
using namespace std;

Socket::Socket()
{
}

Socket::~Socket()
{
    	close(socketfd);
    	cout<<"socket closed"<<endl;
}

void Socket::Bind()
{
		int b=::bind(socketfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
		EXIT_IF(b<0, "bind wrong\n, errno:%d %s",errno, strerror(errno));
}

void Socket::Listen()
{
		int l=listen(socketfd, BACKLOG);
		EXIT_IF(l<0, "listen wrong\n");
}

void Socket::initilize_listen()
{
		short port=PORT;
		socketfd=socket(AF_INET, SOCK_STREAM, 0);
		EXIT_IF(socketfd<0, "socket wrong\n");
		sock_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		sock_addr.sin_port=htons(PORT);
		sock_addr.sin_family=htonl(AF_INET);
    	Bind();
    	Listen();
		set_non_blocking();
    	cout<<"listening socket bind at: "<<"127.0.0.1:"<<ntohs(sock_addr.sin_port)<<"and keep listening!"<<endl;
}
void Socket::set_non_blocking()                                             
{
        int opts;
        opts = fcntl(socketfd, F_GETFL,0);
        EXIT_IF(opts<0,"fcntl GET wrong\n");
        opts = (opts | O_NONBLOCK);
        EXIT_IF(fcntl(socketfd, F_SETFL, opts) < 0, "fcntl SET wrong\n");
}     
int main()
{
    	class Socket my_socket;
    	my_socket.initilize_listen();
    	cout<<"socket init"<<endl;
		for(;;)
		{

		}
    	return 0;
}
