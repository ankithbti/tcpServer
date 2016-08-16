/*
 * Connection.hpp
 *
 *  Created on: 16-Aug-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_CONNECTION_HPP_
#define INCLUDE_CONNECTION_HPP_

#include <iostream>
#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

namespace amt{

class ConnectionManager;

class Connection : private boost::noncopyable, public boost::enable_shared_from_this<Connection>{

	boost::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	boost::array<char, 1024> _buffer;
	std::string _clientId;
	boost::shared_ptr<ConnectionManager> _manager;

	void asyncRead(boost::system::error_code ec, std::size_t bytes_transferred)
	{
		std::cout << " Read " << bytes_transferred << " bytes sent from client - " << _buffer.data() << std::endl;
	}

	void do_read()
	{
		_socket->async_read_some(boost::asio::buffer(_buffer), boost::bind(&Connection::asyncRead, this->shared_from_this(), _1, _2));
	}


	void asyncWrite(boost::system::error_code ec, std::size_t bytes_transferred);

public:

	Connection(boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
			boost::shared_ptr<ConnectionManager> manager) :
				_socket(socket),
				_manager(manager){

	}

	~Connection(){
		if(_socket){
			if(_socket->is_open()){
				_socket->close();
			}
			_socket.reset();
		}
	}

	void write(char* msg, int len)
	{
		boost::asio::async_write(*(_socket.get()), boost::asio::buffer(msg, len),
				boost::bind(&Connection::asyncWrite, this->shared_from_this(), _1, _2));
	}

	void start()
	{
		std::cout << " Starting Client Connection. " << std::endl;
		do_read();
	}

	void stop()
	{
		_socket->close();
		_socket.reset();
	}
};
using ConnectionPtr = boost::shared_ptr<Connection>;

}



#endif /* INCLUDE_CONNECTION_HPP_ */
