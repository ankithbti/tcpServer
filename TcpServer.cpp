#include <TcpServer.hpp>

#include <iostream>
namespace amt
{

  TcpServer::TcpServer ()
  {

    _epfd = epoll_create (256); // for handling accept

    _listenfd = socket (PF_INET, SOCK_STREAM, 0);

    setNonBlocking (_listenfd);

    _ev.data.fd = _listenfd;
    _ev.events = EPOLLIN | EPOLLET;

    epoll_ctl (_epfd, EPOLL_CTL_ADD, _listenfd, &_ev);

  }

void  TcpServer::connect (char* host, int port)
  {
    bzero (&_serverAddr, sizeof (_serverAddr));
    _serverAddr.sin_family = AF_INET;
    if (host)
      {
        inet_aton (host, &(_serverAddr.sin_addr));
      }
    else
      {
        _serverAddr.sin_addr.s_addr = INADDR_ANY;
      }

    _serverAddr.sin_port = htons (port);

    bind (_listenfd, (sockaddr*) & _serverAddr, sizeof (_serverAddr));
    listen (_listenfd, LISTENQ);
  }

  void TcpServer::run ()
  {

    for (;;)
      {
        _nfds = epoll_wait (_epfd, _events, LISTENQ, TIMEOUT);

        for (int i = 0; i < _nfds; ++i)
          {
            if (_events[i].data.fd == _listenfd)
              {
                // New Connection connection comes

                _connfd = accept (_listenfd, (sockaddr*) & _clientAddr, &_clientSockLen);
                if (_connfd < 0)
                  {
                    perror ("Failed to accept client connection.");
                  }
                setNonBlocking (_connfd);
                std::cout << " Connection accepted from client : " << inet_ntoa (_clientAddr.sin_addr) << ":" << _clientAddr.sin_port << " @ " << _connfd << std::endl;
                _ev.data.fd = _connfd;
                _ev.events = EPOLLIN | EPOLLET;
                epoll_ctl (_epfd, EPOLL_CTL_ADD, _connfd, &_ev);

              }
            else if (_events[i].events && EPOLLIN)
              {
                // Data Read
                std::cout << " Got a read req from " << _events[i].data.fd << std::endl;
                if (_events[i].data.fd < 0)
                  {
                    continue;
                  }

              }
            else if (_events[i].events && EPOLLOUT)
              {
                // Have data to send
                std::cout << " Got a read req from " << _events[i].data.fd << std::endl;
                if (_events[i].data.fd < 0)
                  {
                    continue;
                  }
              }else{
                std::cout << " Unknown Poll event. " << std::endl;
                }
          }

      }



  }

  void TcpServer::setNonBlocking (int sock) {
    
    int opts;
    if((opts = fcntl (sock, F_GETFL)) < 0){
        perror ("Failed to run GETFL");
      }
    opts = opts | O_NONBLOCK;
    if(fcntl (sock, F_SETFL, opts) < 0){
        perror("failed to set NONBLOCKING option.");
      }
 }



}
