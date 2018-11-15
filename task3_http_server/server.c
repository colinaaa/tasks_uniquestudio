#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#define BACKLOG 10
#define PORT 9998

int Socket(int domain, int type, int protocal);

int main(void)
{
		printf("hello\n");
		int listenfd, connectfd;
		struct sockaddr_in servaddr, cliaddr;
		int sin_size;

		listenfd=Socket(AF_INET, SOCK_STREAM, 0);
		printf("listenfd%d\n",listenfd);
		servaddr.sin_family=AF_INET;
		servaddr.sin_port=htons(PORT);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

		bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

		listen(listenfd,BACKLOG);
		while (1)
		{

				sin_size=sizeof(cliaddr);
				connectfd=accept(listenfd,(struct sockaddr*)&cliaddr, &sin_size);
				printf("got connection from %s\n",inet_ntoa(cliaddr.sin_addr));
				send(connectfd,"hello, world",14,0);
		}
		close(listenfd);
		exit(1);
		return 0;

}

int Socket(int domain, int type, int protocal)
{
		int sockfd;
		if((sockfd=socket(domain,type,protocal))<0)
		{
				printf("socket wrong\n");
		}
		return sockfd;
}

