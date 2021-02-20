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
#include <wincrypt.h>

class manager
{
public:
	std::vector<char> badchars = {
		'\'',
		'@',
		'#',
		'$',
		'%',
		'^',
		'=',
		'/',
		'\\',
		'`',
		':',
		';',
		'~',
		'<',
		'>',
		'?',
		'/'
	};
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
	Order getOrderuid(int userid);
	bool doesOrderExist(std::string orderid);
	bool doesDuidExist(std::string duid);
	bool isOrderRedeemed(std::string orderid);
	std::string getHwid(std::string uid);
	std::string hash(const void* data, const size_t data_size, HashType hashType);
	int getUid(std::string uid); //username
	bool doesUsernameExist(std::string username);
	bool doesUidExist(int uid);
	bool isOrderPaidFor(std::string orderid);
	void addOrder(Order info);
	void redeemOrder(std::string orderid, std::string username);
	void addUser(std::string username, std::string pwdhash, int accessLevel, std::string hwid);
	void addUser(std::string username, std::string pwdhash, int accessLevel, std::string hwid, std::string duid);
	void removeUser(std::string username);
	void removeOrder(std::string orderid);
	void setHwid(int uid, std::string hwid);
	int isAccountLocked(std::string username);
	std::string getLockedReason(std::string username);
	void unlockAccount(std::string username);
	std::string getAllUids();
	std::string getUname(int uid);
	std::string getSubDate(int uid);
	std::string getSubEndDate(int uid);
	void setSubDate(std::string date, int accessLevel, int uid);
	void expiredUser(int uid);

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