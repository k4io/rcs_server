#pragma once
#include "includes.h"
#include "mysql_connection.h"
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "cppconn\exception.h"
#include "cppconn\prepared_statement.h"
#include "cppconn\statement.h"

class manager
{
public:
	sql::Driver* driver;
	sql::Connection* conn;
	sql::Statement* stmt;
	sql::ResultSet* res;
	manager();
	~manager();
	std::string ExecuteNonQuery(std::string query);
	std::string ExecuteQuery(std::string query, std::string column);
	int ExecuteQueryInt(std::string query, std::string column);
	bool connect();
	std::vector<Order> getAllOrders();
	Order getOrder(std::string orderid);
	Order getOrderuid(std::string userid);
	bool doesOrderExist(std::string orderid);
	bool isOrderRedeemed(std::string orderid);
	std::string getHwid(std::string uid);
	int getUid(std::string uid);
	bool doesUsernameExist(std::string username);
	bool isOrderPaidFor(std::string orderid);
	void addOrder(Order info);
	void redeemOrder(std::string orderid, std::string username);

	std::vector<std::string> explode(const std::string& s, const char& c)
	{
		std::string buff{ "" };
		std::vector<std::string> v;

		for (auto n : s)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if (buff != "") v.push_back(buff);

		return v;
	}
private:
};