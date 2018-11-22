#include"server.h"
#include<iostream>
using namespace std;

Socket::~Socket()
{
    	close(get_fd());
		if(TEST_OUTPUT)
    			cout<<"socket obj closed"<<endl;
}
Socket::Socket(int fd, struct sockaddr_in* addr):socketfd(fd), sock_addr(*addr)
{
		cout<<"init socket"<<endl;
}//有fd和地址的构造函数(for connect socket)
void set_non_blocking(int fd)
{
        int opts;
        opts = fcntl(fd, F_GETFL,0);
        EXIT_IF(opts<0,"fcntl GET wrong\n");
        opts = (opts | O_NONBLOCK);
        EXIT_IF(fcntl(fd, F_SETFL, opts) < 0, "fcntl SET wrong\n");
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

ConnectSocket ListenSocket::Accept()
{
		int connectfd;
		struct sockaddr_in cliaddr;
		socklen_t len=sizeof(cliaddr);
		connectfd=accept(get_fd(), (struct sockaddr*)&cliaddr, &len);
		EXIT_IF(connectfd<0, "accept wrong\n");
		set_non_blocking();
		//create connecting socket
		class ConnectSocket consocket(connectfd, &cliaddr);
		return consocket;
}

ConnectSocket::ConnectSocket(int fd, struct sockaddr_in* addr):Socket(fd, addr)
{
		if(TEST_OUTPUT)
				cout<<"got connection from"<<inet_ntoa(get_addr()->sin_addr)<<":"<<ntohs(get_addr()->sin_port)<<endl;
}

Epoll::Epoll()
{
		epollfd=epoll_create(EPOLL_SIZE);
		maxevs=MAXEVENTS;
}

void Epoll::event_add(int fd, int state)
{
		struct epoll_event ev;
		ev.data.fd=fd;
		ev.events=state|EPOLLET;
		fd_ev[fd]=ev;
		int r=epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
		EXIT_IF(r<0, "event add wrong\n");
		if(TEST_OUTPUT)
				cout<<"event added,"<<"fd: "<<fd<<endl;
}

void Epoll::event_delete(int fd)
{
		//struct epoll_event ev;
		//ev.data.fd=fd;
		//ev.events=state|EPOLLET;
		struct epoll_event ev=fd_ev[fd];
		int r=epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
		EXIT_IF(r<0, "event delete wrong\n");
		if(TEST_OUTPUT)
				cout<<"event deleted"<<endl;
}

void Epoll::event_modify(int fd, int state)
{
		//struct epoll_event ev;
		struct epoll_event ev=fd_ev[fd];
		ev.events=state|EPOLLET;
		if(TEST_OUTPUT)
				cout<<"modifing"<<endl;
		int r=epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
		EXIT_IF(r<0,"modify wrong\n");
}
char* Epoll::e_read(int fd, char* buf)
{
		int nread;
		int n=0;
		size_t nleft=MAXBUFFER;
		while ((nread=read(fd, buf+n, READ_SIZE))>0)
		{
				n+=nread;
				if(TEST_OUTPUT)
						cout<<"read: "<<nread<<endl;
		}
		if(nread==-1&&errno!=EAGAIN)
		{
				cout<<"read wrong"<<endl;
				event_delete(fd);
				close(fd);
		}
		else if(nread==0)
		{
				cout<<"client close"<<endl;
				event_delete(fd);
				close(fd);
		}
		if(TEST_OUTPUT)
				printf("receive:%s",buf);
		return buf;
		//event_modify(fd, EPOLLOUT);在httpser中转换
}

int Epoll::e_write(int fd, const char* buf)
{
		int nwrite;
		size_t data_size=MAXBUFFER;
		int n=data_size;
		cout<<"write fd:"<<fd<<endl;
		while(n>0)
		{
				nwrite=(write(fd, buf+data_size-n, n));
				if(nwrite<0&&errno!=EAGAIN)
				{
						cout<<"write wrong"<<endl;
						event_delete(fd);
						close(fd);
						if(TEST_OUTPUT)
								cout<<"connectfd close"<<endl;
						break;
				}
				n-=nwrite;
		}
		return 0;
}

HTTPServer::HTTPServer(int listenfd, struct sockaddr_in* servaddr):listensocket(listenfd, servaddr)
{
		if(TEST_OUTPUT)
				cout<<"server build"<<endl;
}

void HTTPServer::loop(Epoll& epoll)
{
		if(TEST_OUTPUT)
				cout<<"waiting..."<<endl;
		int ready_events=epoll_wait(epoll.epollfd, epoll.active_evs, epoll.maxevs, -1);
		if(TEST_OUTPUT)
				cout<<ready_events<<"events ready"<<endl;
		handle_events(epoll, ready_events);
}

void HTTPServer::handle_accept(Epoll& epoll)
{
		int connectfd;
		struct sockaddr_in cliaddr;
		socklen_t len=sizeof(cliaddr);
		connectfd=accept(listensocket.get_fd(), (struct sockaddr*)&cliaddr, &len);
		set_non_blocking(connectfd);
		cout<<"got connection from"<<inet_ntoa(cliaddr.sin_addr)<<":"<<ntohs(cliaddr.sin_port)<<endl;
		addr_map[connectfd]=cliaddr;
		epoll.event_add(connectfd, EPOLLIN);
}

void HTTPServer::handle_events(Epoll& epoll, int ready_events)
{
		int i;
		for(i=0; i<ready_events; i++)
		{
				int fd=epoll.active_evs[i].data.fd;
				cout<<"handle fd:"<<fd<<endl;
				int events=epoll.active_evs[i].events;
				if((fd==listensocket.get_fd())&& events&EPOLLIN)
				{
						handle_accept(epoll);
						if(TEST_OUTPUT)
								cout<<"handling accept"<<endl;
				}
				else if(events&EPOLLIN)
				{
						parse_request(epoll, fd);
						if(TEST_OUTPUT)
								cout<<"handling read"<<endl;
				}
				else if(events&EPOLLOUT)
				{
						make_response(epoll, fd);
						if(TEST_OUTPUT)
								cout<<"handling write"<<endl;
				}
		}
}

int HTTPServer::parse_request(Epoll& epoll, int connectfd)
{
		char read_buf[MAXBUFFER];
		bzero(read_buf,MAXBUFFER);
		if(TEST_OUTPUT)
				cout<<"parsing"<<endl;
		epoll.e_read(connectfd, read_buf);
		if(TEST_OUTPUT)
				printf("read:%s\n",read_buf);
		epoll.event_modify(connectfd, EPOLLOUT);
		///////////////////
		////unimplement////
		///////////////////
		return 0;
}

void HTTPServer::make_response(Epoll& epoll, int connectfd)
{
		//req=reqs_map[connectfd];
		char write_buf[MAXBUFFER]="HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
		epoll.e_write(connectfd, write_buf);
		if(TEST_OUTPUT)
				cout<<"made response"<<endl;
		//keep-alive
		epoll.event_delete(connectfd);
		close(connectfd);
}

int main()
{
		signal(SIGPIPE, SIG_IGN);
		int listenfd=socket(AF_INET, SOCK_STREAM, 0);
		EXIT_IF(listenfd<0,"socket wrong\n");
		struct sockaddr_in servaddr;
		servaddr.sin_port=htons(PORT);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		servaddr.sin_family=AF_INET;
		//create listening socket
		class ListenSocket listensocket(listenfd, &servaddr);
		listensocket.init();
		//create epoll
		class Epoll epoll;
		epoll.event_add(listenfd, EPOLLIN);
		//create server
		class HTTPServer my_server(listenfd, &servaddr);

		for(;;)
		{
				my_server.loop(epoll);
		}
    	return 0;
}
