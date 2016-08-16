/*
 * Connection.cpp
 *
 *  Created on: 17-Aug-2016
 *      Author: ankithbti
 */

#include <Connection.hpp>
#include <ConnectionManager.hpp>

namespace amt{

void Connection::asyncWrite(boost::system::error_code ec, std::size_t bytes_transferred)
{
	if (!ec)
	{
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both,
				ignored_ec);
	}

	if (ec != boost::asio::error::operation_aborted)
	{
		_manager->stop(this->shared_from_this());
	}
}
}


