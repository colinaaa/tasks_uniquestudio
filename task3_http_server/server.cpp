#include"server.h"
#include<iostream>
using namespace std;

Socket::Socket(int fd, struct sockaddr_in* addr):socketfd(fd), sock_addr(*addr){}//有fd和地址的构造函数(for connect socket)

Socket::~Socket()
{
    	close(get_fd());
    	cout<<"socket closed"<<endl;
}

void Socket::set_non_blocking()
{
        int opts;
        opts = fcntl(get_fd(), F_GETFL,0);
        EXIT_IF(opts<0,"fcntl GET wrong\n");
        opts = (opts | O_NONBLOCK);
        EXIT_IF(fcntl(get_fd(), F_SETFL, opts) < 0, "fcntl SET wrong\n");
}

ListenSocket::ListenSocket(int listenfd, struct sockaddr_in* servaddr):Socket(listenfd, servaddr)
{
		cout<<"create a listen socket obj\n";
}
void ListenSocket::init()
{
		Bind(get_fd(), get_addr());
		Listen(get_fd());
    	cout<<"listening socket bind at: "<<"127.0.0.1:"<<ntohs(get_addr()->sin_port)<<"and keep listening!"<<endl;
}
void ListenSocket::Bind(int listenfd, struct sockaddr_in* addr)
{
		int b=::bind(listenfd, (struct sockaddr*)addr, sizeof(struct sockaddr));
		EXIT_IF(b<0, "bind wrong\n, errno:%d %s",errno, strerror(errno));
}

void ListenSocket::Listen(int listenfd)
{
		int l=listen(listenfd, BACKLOG);
		EXIT_IF(l<0, "listen wrong\n");
}

int main()
{
		int listenfd=socket(AF_INET, SOCK_STREAM, 0);
		EXIT_IF(listenfd<0,"socket wrong\n");
		struct sockaddr_in servaddr;
		servaddr.sin_port=htons(PORT);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		servaddr.sin_family=AF_INET;
		class ListenSocket listensocket(listenfd, &servaddr);
		listensocket.init();
		for(;;)
		{

		}
    	return 0;
}
