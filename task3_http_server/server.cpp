#include"server.h"
#include<fstream>
#include<iostream>
using namespace std;

Socket::~Socket()
{
    	close(get_fd());
		if(TEST_OUTPUT)
		{
    			cout<<"socket obj closed"<<endl;
		}
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
		{
				cout<<"create a listen socket obj\n";
		}
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
		{
				cout<<"got connection from"<<inet_ntoa(get_addr()->sin_addr)<<":"<<ntohs(get_addr()->sin_port)<<endl;
		}
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
		{
				cout<<"event added,"<<"fd: "<<fd<<endl;
		}
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
		{
				cout<<"event deleted"<<endl;
		}
}

void Epoll::event_modify(int fd, int state)
{
		//struct epoll_event ev;
		struct epoll_event ev=fd_ev[fd];
		ev.events=state|EPOLLET;
		if(TEST_OUTPUT)
		{
				cout<<"modifing"<<endl;
		}
		int r=epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
		EXIT_IF(r<0,"modify wrong\n");
}
int Epoll::e_read(int fd, char* buf)
{
		int nread;
		int n=0;
		size_t nleft=MAXBUFFER;
		while ((nread=read(fd, buf+n, READ_SIZE))>0)
		{
				n+=nread;
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
		{
				printf("receive:%s\n",buf);
		}
		return n;
		//event_modify(fd, EPOLLOUT);在httpser中转换
}

int Epoll::e_write(int fd, const char* buf, int data_size)
{
		int nwrite;
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
						{
								cout<<"connectfd close"<<endl;
						}
						break;
				}
				n-=nwrite;
		}
		return 0;
}

HTTPServer::HTTPServer(int listenfd, struct sockaddr_in* servaddr):listensocket(listenfd, servaddr)
{
		if(TEST_OUTPUT)
		{
				cout<<"server build"<<endl;
		}
}

void HTTPServer::loop(Epoll& epoll)
{
		if(TEST_OUTPUT)
		{
				cout<<"waiting..."<<endl;
		}
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
		int read_bytes=epoll.e_read(connectfd, read_buf);
		if(read_bytes<=0)
		{
				return 0;
		}
		char method[10];
		char path[1024];
		char protocol[100];
		sscanf(read_buf,"%s %s %s", method, path, protocol);
		string raw_req=read_buf;
		HttpReq req;
		if(raw_req.length()>=5)//GET /
		{
				req.method=method;
				req.path=path;
				size_t body=raw_req.find("\r\n\r\n");
				req.body=raw_req.substr(body+4);
				size_t header=raw_req.find("\r\n")+2;
				string headers=raw_req.substr(header);
				while(!headers.empty())
				{
						//cout<<headers<<endl;
						size_t i=headers.find(":");
						size_t j=headers.find("\r\n")+2;
						if(i==string::npos||j==string::npos)
						{
								break;
						}
						//cout<<"i: "<<i<<"j:"<<j<<endl;
						//cout<<"k: "<<headers.substr(0,i)<<endl;
						//cout<<"v: "<<headers.substr(i+2,j-i-4)<<endl;
						req.Headers[headers.substr(0,i)]=headers.substr(i+2, j-i-4);
						headers.erase(0,j);
				}
		}
		else
		{
				req.method="broken";
		}
		reqs_map[connectfd]=req;
		epoll.event_modify(connectfd, EPOLLOUT);
		return 0;
}

void HTTPServer::make_response(Epoll& epoll, int connectfd)
{
		HttpReq req;
		req=reqs_map[connectfd];
		class HTTPRes res;
		bool succeed;
		if(req.method=="GET")
		{
				succeed=res.do_get(req.path);
		}
		else if(req.method=="POST")
		{
				succeed=res.do_post(req.body, req.path);
		}
		else if(req.method=="DELETE")
		{
				succeed=res.do_delete(req.path);
		}
		else if(req.method=="broken")
		{
				res.status_code=400;
				res.phrase="Bad Request";
				res.body="broken\n";
				succeed=false;
		}
		else
		{
				res.status_code=405;
				res.phrase="Method not allowed";
				res.body="Not allowed\n";
				succeed=false;
		}
			if(succeed)
		{
				res.status_code=200;
				res.phrase="OK";
		}
		res.set_header("Content-Type", "text/html")
				.set_header("Server", "abc")
				.set_header("Content-Length", to_string(res.count_length()));
		string Response=res.join_res();
		int res_len=Response.length();
		char write_buf[res_len+1];
		strcpy(write_buf, Response.c_str());
		if(TEST_OUTPUT)
		{
				printf("response message:%s\n", write_buf);
		}
		epoll.e_write(connectfd, write_buf, res_len);
		if(TEST_OUTPUT)
		{
				cout<<"made response"<<endl;
		}
		if(req.Headers["Connection"]=="close")
		{
				res.set_header("Connection", "close");
				epoll.event_delete(connectfd);
				close(connectfd);
		}
		else
		{
				res.set_header("Connection", "keep-alive")
						.set_header("Keep-Alive", "timeout=5,max=1000");
				epoll.event_modify(connectfd, EPOLLIN);
		}
//keep-alive
}

HTTPRes& HTTPRes::set_header(string key, string value)
{
		string Header;
		Header=key+": "+value;
		if(TEST_OUTPUT)
		{
				cout<<"set_header: "+Header<<endl;
		}
		headers.push_back(Header);
		return *this;
}

bool HTTPRes::do_get(const string path)
{
		string root=ROOT;
		ifstream fin(root+path);
		if(!fin.is_open())
		{
				status_code=404;
				phrase="Not Found";
				body="try again\n";
				fin.close();
				return false;
		}
		fin>>body;
		fin.close();
		return true;
}

bool HTTPRes::do_post(const string data, const string path)
{
		string root=ROOT;
		//设置文件模式(app为追加至文件尾)
		ofstream fout(root+path, ios_base::out|ios_base::app);
		fout<<data;
		fout.close();
		//write data to root+path
		return true;
}

bool HTTPRes::do_delete(const string path)
{
		string root=ROOT;
		ofstream fout(root+path, ios_base::out|ios_base::trunc);//截短文件
		fout<<" ";
		fout.close();
		//delete data to root+path
		body="deleted\n";
		return true;
}

int HTTPRes::count_length()
{
		return (int)body.length();
}

const string HTTPRes::join_res()
{
		string Headers="";
		for(int i=0;i<headers.size();i++)
		{
				Headers+=headers[i]+"\r\n";
		}
		if(TEST_OUTPUT)
		{
				cout<<"join: "<<Headers<<endl;
		}
		return "HTTP/1.1 "+to_string(status_code)+" "+phrase+"\r\n"+Headers+"\r\n"+body;
}
void quitHandler(int sig)
{
		cout<<"reveive ctrl+c"<<endl;
		exit(EXIT_SUCCESS);
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
				signal(SIGINT, quitHandler);
		}
    	return 0;
}
