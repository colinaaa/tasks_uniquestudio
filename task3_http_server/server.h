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

#define BACKLOG 10
#define PORT 9998
#define EPOLL_SIZE 4096
#define READ_SIZE 1024

void set_non_blocking(int);
void add_event(int, int, int);
void event_modify(int, int, int);
void event_delete(int, int, int);
void handle_events(int, int, struct epoll_event*, int, char*);
int e_read(int, int, char*);
int e_write(int, int, char*);
int handle_accept(int, int);
int Socket(int, int, int);
int Bind(int, struct sockaddr*, socklen_t);
int Listen(int, int);
#endif
