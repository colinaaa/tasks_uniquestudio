#include"server.h"
#include<signal.h>

int main(void)
{
		printf("hello\n");
		int listenfd, connectfd;
		struct sockaddr_in servaddr, cliaddr;
		struct epoll_event events[100];//用来存放events(obviously...)
		int epollfd;
		int ready_events;//epoll中准备好的events个数
		
		sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);//忽略SIGPIPE错误（向已关闭的fd中write）暂时还没想到更好的解决办法。。stack overflow上找了这个。。
		listenfd=Socket(AF_INET, SOCK_STREAM, 0);
		set_non_blocking(listenfd);
		printf("listenfd%d\n",listenfd);
		servaddr.sin_family=AF_INET;
		servaddr.sin_port=htons(PORT);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		Listen(listenfd,BACKLOG);

		epollfd=epoll_create(EPOLL_SIZE);
		add_event(epollfd,listenfd,EPOLLIN);//use ET(edge-triggered)

		for(;;)
		{
				printf("waiting...\n");
				ready_events=epoll_wait(epollfd, events, 100, -1);
				printf("%devents ready\n",ready_events);
				handle_events(epollfd, listenfd, events, ready_events);
		}
		return 0;

}

void set_non_blocking(int listenfd)
{
		int opts;    
		if((opts = fcntl(listenfd, F_GETFL,0))<0)
		{
				printf("fcntl wrong\n");
				return;
		}	
		opts = (opts | O_NONBLOCK);    
		if(fcntl(listenfd, F_SETFL, opts) < 0)
		{
				printf("fcntl2 wrong\n");
		}
}

void add_event(int epollfd, int fd, int state)
{
		struct epoll_event ev;//ev用来注册事件
		ev.events=state|EPOLLET;
		ev.data.fd=fd;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);//将fd添加至epoll监测
}

void handle_events(int epollfd, int listenfd, struct epoll_event* events, int ready_events)
{
		int i;
		for(i=0;i<ready_events;i++)
		{
				int fd;
				fd=events[i].data.fd;
				if((fd==listenfd)&&(events[i].events&EPOLLIN))
				{
						handle_accept(listenfd, epollfd);
				}
				else if(events[i].events&EPOLLIN)
				{
						e_read(epollfd, fd);
				}
				else if(events[i].events&EPOLLOUT)
				{
						e_write(epollfd, fd);
				}
		}
}

int handle_accept(int listenfd, int epollfd)
{
		int connectfd;
		struct sockaddr_in cliaddr;
		socklen_t addrlen=sizeof(cliaddr);
		if((connectfd=accept(listenfd, (struct sockaddr*)&cliaddr, &addrlen))==-1)
		{
				printf("accept wrong\n");
		}
		else
		{
				printf("got connetion form %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
				set_non_blocking(connectfd);
				add_event(epollfd, connectfd, EPOLLIN);
		}
}

int e_read(int epollfd, int fd)
{
		int nread;
		char buf[MAXBUFFER];
		int n=0,i=0;
		size_t nleft=sizeof(buf);
		while((nread=read(fd,buf+n,READ_SIZE))>0)//精妙的指针操作。。
		{
				printf("read time:%d\n",i++);
				printf("nread=%d\n",nread);
				n+=nread;
		}
		if(nread==-1&&errno!=EAGAIN)
		{
				printf("readfd close\n");
				event_delete(epollfd, fd, EPOLLIN);
				close(fd);
				printf("read wrong\n");
		}
		else if(nread==0)
		{
				printf("client close\n");
				event_delete(epollfd, fd, EPOLLIN);
				close(fd);
		}
		printf("%s\n",buf);
		event_modify(epollfd, fd, EPOLLOUT);
}

int e_write(int epollfd, int fd)
{
		char buf[MAXBUFFER];
		bzero(buf,MAXBUFFER);
		int nwrite, data_size=sizeof(buf);
		int n=data_size;
		while(n>0)
		{
				sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World!",12);//make_response
				nwrite=write(fd, buf+data_size-n, n);//更精妙的指针操作。。
				if (nwrite<n)//写入的少于buf长度
				{
						if (nwrite==-1&&errno!=EAGAIN)
						{
								printf("writefd close\n");
								event_delete(epollfd, fd, EPOLLOUT);
								close(fd);
								printf("write error\n");
								break;
						}
				}
				n-=nwrite;
		}
}

void event_modify(int epollfd, int fd, int state)
{
		struct epoll_event ev;
		ev.data.fd=fd;
		ev.events=state | EPOLLET;
		printf("modifing\n");
		if(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev)<0)
		{
				printf("modify wrong\n");
		}
}

void event_delete(int epollfd, int fd, int state)
{
		struct epoll_event ev;
		ev.data.fd=fd;
		ev.events=state ;
		if(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev)<0)
		{
				printf("delete wrong\n");
		}
}

int Socket(int domain, int type, int protocal)
{
		int sockfd;
		if((sockfd=socket(domain,type,protocal))==-1)
		{
				printf("socket wrong\n");
		}
		return sockfd;
}

int Bind(int sockfd, struct sockaddr* servaddr, socklen_t addrlen)
{
		if (bind(sockfd,servaddr, addrlen)==-1)
		{
				printf("bind wrong\n");
		}
}

int Listen(int sockfd, int backlog)
{
		if (listen(sockfd, backlog)==-1)
		{
				printf("listen wrong\n");
		}
}
