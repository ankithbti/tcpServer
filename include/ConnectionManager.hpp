/*
 * ConnectionManager.hpp
 *
 *  Created on: 16-Aug-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_CONNECTIONMANAGER_HPP_
#define INCLUDE_CONNECTIONMANAGER_HPP_

#include <Connection.hpp>
#include <set>
#include <boost/foreach.hpp>

namespace amt{

class ConnectionManager {
public:
	typedef std::set<ConnectionPtr> Connections;
	typedef Connections::iterator ConnectionsIt;

private:
	Connections _connections;

public:

	void start(ConnectionPtr conn){
		conn->start();
		_connections.insert(conn);
	}

	void stop(ConnectionPtr conn){
		conn->stop();
		_connections.erase(conn);
	}

	void stopAll(){
		for(ConnectionsIt it = _connections.begin() ; it != _connections.end() ; ++it){
			(*it)->stop();
		}
		_connections.clear();
	}
};
using ConnectionManagerPtr = boost::shared_ptr<ConnectionManager>;
}



#endif /* INCLUDE_CONNECTIONMANAGER_HPP_ */
