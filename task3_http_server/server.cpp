#include"server.h"
#include<iostream>
using namespace std;

Socket::Socket(int fd, struct sockaddr_in* addr):socketfd(fd), sock_addr(*addr){}//有fd和地址的构造函数(for connect socket)

Socket::~Socket()
{
    	close(get_fd());
		if(TEST_OUTPUT)
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
		if(TEST_OUTPUT)
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
		EXIT_IF(b<0, "bind wrong\n");
}

void ListenSocket::Listen(int listenfd)
{
		int l=listen(listenfd, BACKLOG);
		EXIT_IF(l<0, "listen wrong\n");
}

ConnectSocket::ConnectSocket(int fd, struct sockaddr_in* addr):Socket(fd, addr);
{
		if(TEST_OUTPUT)
				std::cout<<"got connection from"<<ntohl(sock_addr.sin_addr.s_addr)<<":"<<ntohs(sock_addr.sin_port)<<std::endl;
};

Epoll::Epoll()
{
		epollfd=epoll_create(EPOLL_SIZE);
		maxevs=MAXEVENTS;
}

void Epoll::event_add(int fd, int state)
{
		//struct epoll_event ev;
		//ev.data.fd=fd;
		//ev.events=state|EPOLLET;
		struct epoll_event* ev=fd_ev[fd];
		ev->events=state|EPOLLET;
		int r=epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, ev);
		EXIT_IF(r<0, "event add wrong\n");
		if(TEST_OUTPUT)
				cout<<"event added"<<endl;
}

void Epoll::event_delete(int fd)
{
		//struct epoll_event ev;
		//ev.data.fd=fd;
		//ev.events=state|EPOLLET;
		struct epoll_event* ev=fd_ev[fd];
		int r=epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, ev);
		EXIT_IF(r<0, "event delete wrong\n");
		if(TEST_OUTPUT)
				cout<<"event deleted"<<endl;
}

void Epoll::event_modify(int fd, int state)
{
		//struct epoll_event ev;
		struct epoll_event* ev=fd_ev[fd];
		ev->events=state|EPOLLET;
		int r=epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, ev);
}
string* Epoll::e_read(int fd)
{
		int nread;
		int n=0;
		char buf[MAXBUFFER];
		size_t nleft=sizeof(buf);
		while ((nread=read(fd, buf+n, READ_SIZE))>0)
		{
				n+=nread;
				if(TEST_OUTPUT)
						cout<<"read: "<<nread<<endl;
		}
		if(nread==-1&&errno!=EAGAIN)
		{
				cout<<"read wrong"<<endl;
				event_delete(fd, EPOLLIN);
				close(fd);
		}
		else if(nread==0)
		{
				cout<<"client close"<<endl;
				event_delete(fd, EPOLLIN);
				close(fd);
		}
		if(TEST_OUTPUT)
				printf("receive:%s",buf);
		//event_modify(fd, EPOLLOUT);在httpser中转换
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
