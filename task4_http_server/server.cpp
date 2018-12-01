#include "server.h"
#include <fstream>
#include <iostream>
using namespace std;

streamSocket::~streamSocket() {
  close(get_fd());
  if (TEST_OUTPUT) {
    cout << "socket obj destructed" << endl;
  }
}
streamSocket::streamSocket(int fd, struct sockaddr_in *addr)
    : sock_addr(*addr), socketfd(fd) {}

streamSocket::streamSocket() {
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  sock_addr.sin_port = htons(PORT);
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_family = AF_INET;
}

void streamSocket::set_non_blocking() {
  int opts;
  opts = fcntl(get_fd(), F_GETFL, 0);
  EXIT_IF(opts < 0, "fcntl GET wrong\n");
  opts = (opts | O_NONBLOCK);
  EXIT_IF(fcntl(get_fd(), F_SETFL, opts) < 0, "fcntl SET wrong\n");
}

socketAcceptor::socketAcceptor(streamSocket &listenSocket)
    : streamSocket(listenSocket) {
  Bind(get_fd(), get_addr());
  Listen(get_fd());
  set_non_blocking();
  cout << "listening socket bind at "
       << "127.0.0.1:" << ntohs(get_addr()->sin_port) << "and keep listening!"
       << endl;
}

// void socketAcceptor::init()
//{
//		Bind(get_fd(), get_addr());
//		Listen(get_fd());
//		set_non_blocking();
//}

void socketAcceptor::Bind(int listenfd, struct sockaddr_in *addr) {
  int b = ::bind(listenfd, (struct sockaddr *)addr, sizeof(struct sockaddr));
  EXIT_IF(b < 0, "bind wrong\n");
}

void socketAcceptor::Listen(int listenfd) {
  int l = listen(listenfd, BACKLOG);
  EXIT_IF(l < 0, "listen wrong\n");
}

void socketAcceptor::Accept(streamSocket &newConnection) {
  struct sockaddr_in cliaddr;
  socklen_t len = sizeof(cliaddr);
  newConnection.set_fd(
      accept(get_fd(), (struct sockaddr *)newConnection.get_addr(), &len));
}

synchronousEventsDemultiplexer::synchronousEventsDemultiplexer() {
  epollfd = epoll_create(EPOLL_SIZE);
}

synchronousEventsDemultiplexer::synchronousEventsDemultiplexer(int size) {
  epollfd = epoll_create(size);
}

int synchronousEventsDemultiplexer::onceWaitLoop() {
  cout << "waiting..." << endl;
  int readyEvents = epoll_wait(epollfd, active_evs, MAXEVENTS, -1);
  return readyEvents;
}

serverAcceptor::serverAcceptor(streamSocket &listenSocket)
    : _socketAcceptor(listenSocket) {
  initiationDispatcher::instance().registerHandler(this, ACCEPT_EVENT);
}

serverAcceptor::serverAcceptor(int fd, struct sockaddr_in *addr)
    : _socketAcceptor(fd, addr) {
  initiationDispatcher::instance().registerHandler(this, ACCEPT_EVENT);
}

int serverAcceptor::handleEvent(eventType et) {
  assert(et == ACCEPT_EVENT);
  streamSocket newConnection;
  _socketAcceptor.Accept(newConnection);
  readHandler *handler = new readHandler(newConnection);
  return 0;
}

int initiationDispatcher::registerHandler(eventHandler_b *eh, eventType et) {
  struct epoll_event ev;
  ev.data.fd = eh->getHandle();
  int r;
  if ((et & READ_EVENT) || (et & ACCEPT_EVENT)) {
    ev.events = EPOLLIN;
    r = epoll_ctl(SED.getEpollFd(), EPOLL_CTL_ADD, eh->getHandle(), &ev);
  } else if (et & WRITE_EVENT) {
    ev.events = EPOLLOUT;
    r = epoll_ctl(SED.getEpollFd(), EPOLL_CTL_MOD, eh->getHandle(), &ev);
  }
  if (TEST_OUTPUT) {
    EXIT_IF(r < 0, "add wrong\n");
  }
  cbHandler[eh->getHandle()] = eh;
  return r;
}

int initiationDispatcher::removeHandler(eventHandler_b *eh, eventType et) {
  struct epoll_event ev;
  ev.data.fd = eh->getHandle();
  if ((et & READ_EVENT) || (et & ACCEPT_EVENT)) {
    ev.events = EPOLLIN;
  } else if (et & WRITE_EVENT) {
    ev.events = EPOLLOUT;
  }
  int r = epoll_ctl(SED.getEpollFd(), EPOLL_CTL_DEL, eh->getHandle(), &ev);
  if (TEST_OUTPUT) {
    EXIT_IF(r < 0, "delete wrong\n");
  }
  return r;
}

int initiationDispatcher::handleEvents() {
  int readyEvents = SED.onceWaitLoop();
  struct epoll_event *evs = SED.getEvents();
  for (int i = 0; i < readyEvents; i++) {
    int fd = evs[i].data.fd;
    eventType et = (eventType)evs[i].events;
    cbHandler[fd]->handleEvent(et);
  }
  return 0;
}

readHandler::readHandler(streamSocket &ps) : peerStream(ps) {
  initiationDispatcher::instance().registerHandler(this, READ_EVENT);
}

typedef struct Con {
  string read;
  int writen;
  bool writeEnable;
  HttpReq req;
  Con() : writen(0), writeEnable(false) {}
} Con;

map<int, Con> connectionMap;

int readHandler::handleEvent(eventType et) {
  assert(et == READ_EVENT);
  int connectfd = peerStream.get_fd();
  char buf[MAXBUFFER];
  int nread;
  while ((nread = ::read(connectfd, buf, sizeof(buf))) > 0) {
    string &read = connectionMap[connectfd].read;
    read.append(buf, nread);
    if (read.length() > 4) {
      if (read.substr(read.length() - 2, 2) == "\n\n" ||
          read.substr(read.length() - 4, 4) == "\r\n\r\n") {
        connectionMap[connectfd].writeEnable = true;
        writeHandler *handler = new writeHandler(peerStream);
        delete this;
      }
    }
  }
  return 0;
}

writeHandler::writeHandler(streamSocket &ps) : peerStream(ps) {
  initiationDispatcher::instance().registerHandler(this, WRITE_EVENT);
}

int writeHandler::handleEvent(eventType et) {
  if (connectionMap[peerStream.get_fd()].writeEnable) {
		  string responseMessage;
		  HTTPServer::makeResponse(peerStream.get_fd(), responseMessage);
			int nwrite;
  }
}

int HTTPServer::parse_request(int connectfd) {
  char method[10];
  char path[1024];
  char protocol[100];
  const char *read_buf = connectionMap[connectfd].read.c_str();
  sscanf(read_buf, "%s %s %s", method, path, protocol);
  string raw_req = read_buf;
  HttpReq req;
  if (raw_req.length() >= 5) // GET /
  {
    req.method = method;
    req.path = path;
    size_t body = raw_req.find("\r\n\r\n");
    req.body = raw_req.substr(body + 4);
    size_t header = raw_req.find("\r\n") + 2;
    string headers = raw_req.substr(header);
    while (!headers.empty()) {
      // cout<<headers<<endl;
      size_t i = headers.find(":");
      size_t j = headers.find("\r\n") + 2;
      if (i == string::npos || j == string::npos) {
        break;
      }
      // cout<<"i: "<<i<<"j:"<<j<<endl;
      // cout<<"k: "<<headers.substr(0,i)<<endl;
      // cout<<"v: "<<headers.substr(i+2,j-i-4)<<endl;
      req.Headers[headers.substr(0, i)] = headers.substr(i + 2, j - i - 4);
      headers.erase(0, j);
    }
  } else {
    req.method = "broken";
  }
  connectionMap[connectfd].req = req;
  return 0;
}

void HTTPServer::makeResponse(int connectfd, string& responseMessage) {
  HttpReq req;
  req = connectionMap[connectfd].req;
  class HTTPRes res;
  bool succeed;
  if (req.method == "GET") {
    succeed = res.do_get(req.path);
  } else if (req.method == "POST") {
    succeed = res.do_post(req.body, req.path);
  } else if (req.method == "DELETE") {
    succeed = res.do_delete(req.path);
  } else if (req.method == "broken") {
    res.status_code = 400;
    res.phrase = "Bad Request";
    res.body = "broken\n";
    succeed = false;
  } else {
    res.status_code = 405;
    res.phrase = "Method not allowed";
    res.body = "Not allowed\n";
    succeed = false;
  }
  if (succeed) {
    res.status_code = 200;
    res.phrase = "OK";
  }
  res.set_header("Content-Type", "text/html")
      .set_header("Server", "abc")
      .set_header("Content-Length", to_string(res.count_length()));
  if (req.Headers["Connection"] == "close") {
    res.set_header("Connection", "close");
    close(connectfd);
  } else {
    res.set_header("Connection", "keep-alive")
        .set_header("Keep-Alive", "timeout=5,max=1000");
  }
  responseMessage=res.join_res();
  if (TEST_OUTPUT) {
		  cout<<"response message: "<<responseMessage;
  }// keep-alive
}

HTTPRes &HTTPRes::set_header(string key, string value) {
  string Header;
  Header = key + ": " + value;
  if (TEST_OUTPUT) {
    cout << "set_header: " + Header << endl;
  }
  headers.push_back(Header);
  return *this;
}

bool HTTPRes::do_get(const string path) {
  string root = ROOT;
  ifstream fin(root + path);
  if (!fin.is_open()) {
    status_code = 404;
    phrase = "Not Found";
    body = "try again\n";
    fin.close();
    return false;
  }
  fin >> body;
  fin.close();
  return true;
}

bool HTTPRes::do_post(const string data, const string path) {
  string root = ROOT;
  //设置文件模式(app为追加至文件尾)
  ofstream fout(root + path, ios_base::out | ios_base::app);
  fout << data;
  fout.close();
  // write data to root+path
  return true;
}

bool HTTPRes::do_delete(const string path) {
  string root = ROOT;
  ofstream fout(root + path, ios_base::out | ios_base::trunc); //截短文件
  fout << " ";
  fout.close();
  // delete data to root+path
  body = "deleted\n";
  return true;
}

int HTTPRes::count_length() { return (int)body.length(); }

const string HTTPRes::join_res() {
  string Headers = "";
  for (unsigned long i = 0; i < headers.size(); i++) {
    Headers += headers[i] + "\r\n";
  }
  if (TEST_OUTPUT) {
    cout << "join: " << Headers << endl;
  }
  return "HTTP/1.1 " + to_string(status_code) + " " + phrase + "\r\n" +
         Headers + "\r\n" + body;
}
void quitHandler(int sig) {
  cout << "receive ctrl+c" << endl;
  exit(EXIT_SUCCESS);
}
int main() {
  signal(SIGPIPE, SIG_IGN);
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  EXIT_IF(listenfd < 0, "socket wrong\n");
  // create listening socket
  class listenSocket listensocket(listenfd, &servaddr);
  listensocket.init();
  // create epoll
  class Epoll epoll;
  epoll.event_add(listenfd, EPOLLIN);
  // create server
  class HTTPServer my_server(listenfd, &servaddr);

  for (;;) {
    my_server.loop(epoll);
    signal(SIGINT, quitHandler);
  }
  return 0;
}
