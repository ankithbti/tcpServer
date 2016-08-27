/* 
 * File:   TcpServer.hpp
 * Author: dev
 *
 * Created on 19 August, 2016, 12:05 AM
 */

#ifndef TCPSERVER_HPP
#define	TCPSERVER_HPP

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>


#define LISTENQ 20
#define OPENMAX 100
#define TIMEOUT 500

namespace amt{
  
  class TcpServer{
    
    int _epfd;
    epoll_event _ev;
    epoll_event _events[LISTENQ];

    void setNonBlocking(int sock);
    
    int _i, _nfds;
    int _listenfd, _connfd;
    
    socklen_t _clientSockLen;
    sockaddr_in _clientAddr, _serverAddr;
    
  public:
    
    TcpServer();
    void connect(char* host, int port);
    void run();
    
    
    
  };
  
  
  
}

#endif	/* TCPSERVER_HPP */

