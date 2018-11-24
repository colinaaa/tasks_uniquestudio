#ifndef _SERVER_H
#define _SERVER_H
#include<sys/socket.h>
#include<sys/types.h>
#include<cstdlib>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<map>
#include<vector>
#include<utility>
#include<string>
#include<signal.h>
#include<string.h>

#define BACKLOG 10
#define PORT 9999
#define MAXBUFFER 1024
#define MAXEVENTS 100 //for epoll
#define EPOLL_SIZE 4096
#define READ_SIZE 1024
#define TEST_OUTPUT 1
#define ROOT "/home/colin/Documents"
#define EXIT_IF(r,...) if(r){\
						printf(__VA_ARGS__);\
						printf("%s:%d\n, errno:%d, msg:%s",__FILE__, __LINE__,errno, strerror(errno));\
						exit(1);}
class ConnectSocket;
class Epoll;
class HTTPServer;
class Socket
{
        private:
            	struct sockaddr_in sock_addr;
            	int socketfd;
		protected:
				void set_non_blocking();
        public:
            	int get_fd(){return socketfd;};
            	struct sockaddr_in* get_addr(){return &sock_addr;};
            	Socket();
				Socket(int fd, struct sockaddr_in* addr);
            	~Socket();
};

class ListenSocket:public Socket
{
		private:
				void Bind(int, struct sockaddr_in*);
				void Listen(int);
        public:
				void init();
				ConnectSocket Accept();
				ListenSocket(){};
				ListenSocket(int, struct sockaddr_in*);
				~ListenSocket(){};
};

class ConnectSocket:public Socket
{
		public:
				ConnectSocket(int, struct sockaddr_in*);
				~ConnectSocket(){};
				ConnectSocket(const ConnectSocket&);
};
typedef struct HttpReq
{
		std::map<std::string, std::string>Headers;
		std::string method;
		std::string path;
		std::string body;
}HttpReq;
class HTTPServer
{
		private:
				ListenSocket listensocket;
				std::map<int, HttpReq>reqs_map;
				std::map<int, struct sockaddr_in>addr_map;
		public:
				void loop(Epoll&);
				void handle_events(Epoll&, int);
				void handle_accept(Epoll&);
				void make_response(Epoll&, int);
				int parse_request(Epoll&, int);
				HTTPServer(int, struct sockaddr_in*);
				~HTTPServer(){};
};

class HTTPRes
{
		private:
				int status_code;
				std::string phrase;
				std::vector<std::string>headers;
				std::string body;
		public:
				friend class HTTPServer;
				HTTPRes& set_header(std::string, std::string);
				bool do_get(const std::string);
				bool do_post(const std::string, const std::string);
				bool do_delete(const std::string);
				int count_length();
				const std::string join_res();
				HTTPRes(){};
				~HTTPRes(){};
};

class Epoll//eventloop
{
		//uses-a relation
		friend class HTTPServer;
		private:
				int epollfd;
				struct epoll_event* active_evs;
				int maxevs;
				std::map<int, epoll_event>fd_ev;
		public:
				int e_read(int, char*);
				int e_write(int, const char*, int);
				void event_delete(int);
				void event_modify(int, int);
				void event_add(int, int);
				Epoll();
				~Epoll(){close(epollfd);};
};

void quitHandler(int sig);
#endif
