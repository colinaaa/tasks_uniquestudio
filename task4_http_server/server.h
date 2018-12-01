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
#include<cassert>

#define BACKLOG 2048
#define PORT 9999
#define MAXBUFFER 65535
#define MAXEVENTS 1024 //for epoll
#define EPOLL_SIZE 4096
#define TEST_OUTPUT 1
#define ROOT "/home/colin/Documents"
#define EXIT_IF(r,...) if(r){\
						printf(__VA_ARGS__);\
						printf("%s:%d\n, errno:%d, msg:%s",__FILE__, __LINE__,errno, strerror(errno));\
						exit(1);}

enum eventType
{
		ACCEPT_EVENT=1,
		READ_EVENT=1<<1,
		WRITE_EVENT=1<<2,
		CLOSE_EVENT=1<<3,
};

class Epoll;
class HTTPServer;
class eventHandler_b;
class socketAcceptor;
class readHandler;
class writeHandler;
class streamSocket
{
        private:
            	struct sockaddr_in sock_addr;
            	int socketfd;
		protected:
				void set_non_blocking();
        public:
            	int get_fd(){return socketfd;};
				void set_fd(int fd){socketfd=fd;};
            	struct sockaddr_in* get_addr(){return &sock_addr;};
            	streamSocket();
				streamSocket(int fd, struct sockaddr_in* addr);
            	~streamSocket();
};

class socketAcceptor:public streamSocket
//replaced the class listensocket
{
		private:
				void Bind(int, struct sockaddr_in*);
				void Listen(int);
		public:
				void init();
				socketAcceptor(){};
				socketAcceptor(streamSocket&);
				socketAcceptor(int fd, struct sockaddr_in * addr):streamSocket(fd, addr){};
				~socketAcceptor(){close(get_fd());};
				void Accept(streamSocket &);
};

class synchronousEventsDemultiplexer
{
		private:
				int epollfd;
				struct epoll_event* active_evs;
		public:
				int addEvent(eventType);
				synchronousEventsDemultiplexer(int);
				synchronousEventsDemultiplexer();
				~synchronousEventsDemultiplexer(){close(epollfd);}
				int onceWaitLoop();
				int getEpollFd(){return epollfd;}
				struct epoll_event* getEvents(){return active_evs;}
};
class initiationDispatcher
{
		private:
				//synchronousEventsDemultiplexer SED(int size=EPOLL_SIZE);
				synchronousEventsDemultiplexer SED;
				initiationDispatcher(){};
				std::map<int, eventHandler_b*>cbHandler;
		public:
				int registerHandler(eventHandler_b *eh, eventType et);
				int removeHandler(eventHandler_b *eh, eventType et);
				//entry point of event loop
				int handleEvents();
				static initiationDispatcher& instance()
				{
						static initiationDispatcher instance;
						return instance;
				}
				initiationDispatcher(initiationDispatcher const&)=delete;
				void operator=(initiationDispatcher const&)=delete;
};
class eventHandler_b
{
		//an abstrat base class for handlers
		//hook methods???????
		public:
				virtual int handleEvent(eventType et)=0;
				virtual int getHandle()=0;
};

class serverAcceptor:public eventHandler_b
{
		private:
				socketAcceptor _socketAcceptor;
		public:
				serverAcceptor(int fd, struct sockaddr_in * addr);
				serverAcceptor(streamSocket& ss);
				virtual ~serverAcceptor()
				///////
				//???//why it must be virtual???
				///////
				{
						delete this;
				}
				int handleEvent(eventType et);
				virtual int getHandle()
				{
						return _socketAcceptor.get_fd();
				}

};

class readHandler:public eventHandler_b
{
		public:
				readHandler(streamSocket&);
				virtual ~readHandler(){};
				int handleEvent(eventType et);
				int getHandle()
				{
						return peerStream.get_fd();
				}
		private:
				streamSocket peerStream;
};

class writeHandler:public eventHandler_b
{
		public:
				writeHandler(streamSocket&);
				~writeHandler(){};
				int handleEvent(eventType et);
				virtual int getHandle()
				{
						return peerStream.get_fd();
				}
		private:
				streamSocket peerStream;
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
				socketAcceptor _socketAcceptor;
				std::map<int, HttpReq>reqs_map;
		public:
				int parse_request(int);
				static void makeResponse(int, std::string&);
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

class Epoll
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
				Epoll();//default
				Epoll(int, int);
				~Epoll(){close(epollfd);};
};

void quitHandler(int sig);
#endif
