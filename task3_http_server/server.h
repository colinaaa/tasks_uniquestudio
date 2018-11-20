#ifndef _SERVER_H
#define _SERVER_H
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>//for strncmp

#define BACKLOG 10
#define PORT 9999
#define MAXBUFFER 1024
#define EPOLL_SIZE 4096
#define READ_SIZE 1024

#define EXIT_IF(r,...) if(r){\
						printf("__VA_ARGS__");\
						printf("%s:%d error no:%d msg:%s",__FILE__, __LINE__, errno, strerror(errno));\
						exit(1);}
class Socket
{
        private:
            int socketfd;
            struct sockaddr_in sock_addr;
            void Bind();
            void Listen();
            void set_non_blocking();
            void initilize_listen();//bind and listen
        public:
            int get_fd(){return socketfd;};
            int get_addr(){return sock_addr.sin_addr.s_addr};
            Socket();
            ~Socket();
};

#endif
