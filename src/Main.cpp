#include <iostream>
#include <TcpServer.hpp>

using namespace amt;

int main(){

	TcpServer server("127.0.0.1", "8765");
	return 0;
}
