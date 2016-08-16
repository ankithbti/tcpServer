/*
 * TcpServer.hpp
 *
 *  Created on: 16-Aug-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_TCPSERVER_HPP_
#define INCLUDE_TCPSERVER_HPP_

#include <ConnectionManager.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace amt{
class TcpServer {

	std::string _host;
	std::string _port;
	boost::asio::io_service _ioService;
	boost::asio::io_service::work _ioWork;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	boost::asio::ip::tcp::acceptor _acceptor;
	boost::shared_ptr<boost::thread> _ioServiceThread;
	boost::mutex _startStopMutex;
	ConnectionManagerPtr _manager;

	void do_accept()
	{
		if (_acceptor.is_open())
		{
			_acceptor.async_accept(*(_socket.get()),
					boost::bind(&TcpServer::asyncAccept, this, _1));
		}
	}

	void asyncAccept(boost::system::error_code ec)
	{
		if (!_acceptor.is_open())
		{
			std::cout << __FUNCTION__ << " :: Acceptor is not open." << std::endl;
			return;
		}

		if (!ec)
		{
			std::cout << __FUNCTION__ << " :: Accepted client connection." << std::endl;
			ConnectionPtr clientConn(new Connection(_socket, _manager));
			_manager->start(clientConn);
			_socket = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_ioService));
		}
		else
		{
			std::cout << __FUNCTION__ << " :: Error while accepting : " << ec.message() << std::endl;
		}

		// Ready to accept new request
		do_accept();
	}

	void runIoService()
	{
		std::cout << __FUNCTION__ << " :: Started ioService." << std::endl;
		_ioService.run();
		std::cout << __FUNCTION__ << " :: Stopped ioService." << std::endl;
	}
public:

	TcpServer(const std::string& host, const std::string& port) :
		_host(host),
		_port(port),
		_ioService(),
		_ioWork(_ioService),
		_socket(new boost::asio::ip::tcp::socket(_ioService)),
		_acceptor(_ioService),
		_manager(new ConnectionManager()){

		boost::asio::ip::tcp::resolver resolver(_ioService);
		boost::asio::ip::tcp::resolver::query query(_host, _port);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		_acceptor.open(endpoint.protocol());
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		_acceptor.bind(endpoint);
		_acceptor.listen();

		do_accept();
	}

	~TcpServer(){
		if (_socket)
		{
			if (_socket->is_open())
			{
				_socket->close();
			}
			_socket.reset();
		}

		// To Do
		stop();
	}

	void start()
	{
		boost::lock_guard<boost::mutex> lock(_startStopMutex);
		std::cout << __FUNCTION__ << " :: Starting TCP Server." << std::endl;
		if (!_ioServiceThread)
		{
			_ioServiceThread = boost::shared_ptr<boost::thread>(
					new boost::thread(&TcpServer::runIoService, this));
		}
		do_accept();
		std::cout << __FUNCTION__ << " :: Started TCP Server." << std::endl;
	}

	void stop()
	{
		boost::lock_guard<boost::mutex> lock(_startStopMutex);

		std::cout << __FUNCTION__ << " :: Stopping TCP Server." << std::endl;

		if(_acceptor.is_open()){
			_acceptor.close();
		}

		_manager->stopAll();
		std::cout << __FUNCTION__ << " :: Stopped TCP Server." << std::endl;

	}



};
}



#endif /* INCLUDE_TCPSERVER_HPP_ */
