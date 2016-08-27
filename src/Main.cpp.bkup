#include <iostream>
#include <TcpServer.hpp>
#include <sqlite3.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/config.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/warning.h>
#include <cppconn/prepared_statement.h>

using namespace amt;

int main(){

	sql::Connection *con = NULL;
	sql::Driver * driver = get_driver_instance();
	if(driver){
		con = driver->connect("tcp://127.0.0.1:3306", "root", "saring");
		if(con){
			std::cout << " Succesfully Connected to DB. " << std::endl;
		}
		sql::Statement * statement = con->createStatement();
		sql::ResultSet  *res;
		sql::PreparedStatement  *prep_stmt;

		//		statement->execute("USE test");
		res = statement->executeQuery("show databases");

		while(res->next()){
			std::cout << res->getString(1)  << std::endl;
		}

		statement->execute("USE test");

		res = statement->executeQuery("show tables");

		while(res->next()){
			std::cout << res->getString(1)  << std::endl;
		}

		res = statement->executeQuery("select * from employee");

		while(res->next()){
			std::cout << res->getString(1) << " " << res->getInt(2) << std::endl;
		}

		prep_stmt = con->prepareStatement("INSERT INTO employee VALUES (?, ?)");
		prep_stmt->setString(1, "ANKIT GUPTA");
		prep_stmt->setInt(2, 30);
		prep_stmt->execute();

		delete prep_stmt;
		delete res;
		delete statement;
	}
	delete con;
	return 0;

	sqlite3 *db;
	int rc = sqlite3_open(0, &db); // in memory
	if(rc){
		std::cout << " Not able to open database." << std::endl;
		return 0;
	}else{
		std::cout << " Opened database. " << std::endl;
	}
	sqlite3_close(db);

	//TcpServer server("127.0.0.1", "8765");
	return 0;
}
