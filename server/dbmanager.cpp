#include "dbmanager.h"

manager::manager()
{
	//connect();
}

manager::~manager()
{

}

bool manager::connect()
{
	//try {
		driver = get_driver_instance();
		conn = driver->connect("tcp://localhost:3306", "root", "");
		//conn = driver->connect("tcp://[::1]:3306", "root", "");
		conn->setSchema("smoke");
		stmt = conn->createStatement();
		return true;
	//}
	//catch (...) { printf("\n\n################\nERROR! Unable to connect to database!"); exit(-1); }
}

std::string manager::ExecuteNonQuery(std::string query)
{
	std::string s = "";
	if (!connect())
		return s;
	stmt->execute(query);
	return s;
}

std::string manager::ExecuteQuery(std::string query, std::string column)
{
	std::string s = "";
	if (!connect())
		return s;
	res = stmt->executeQuery(query); 

	while (res->next())
		s += res->getString(column) + "\n";
	return s;
}

int manager::ExecuteQueryInt(std::string query, std::string column)
{
	std::string s = "";
	if (!connect())
		return -69;
	res = stmt->executeQuery(query);

	while (res->next())
		return res->getInt(column);
}

Order manager::getOrder(std::string orderid)
{
	std::string s = "SELECT * FROM orders WHERE orderid='";
	s += orderid;
	s += "';";
	std::string result = ExecuteQuery(s, "*");
	std::vector<std::string> v{ explode(result, '\n') };
	Order _n = {
		v[0],
		v[1],
		stoi(v[2]),
		v[3],
		stoi(v[4])
	};
	return _n;
}

Order manager::getOrderuid(std::string userid)
{
	std::string s = "SELECT * FROM orders WHERE userid='";
	s += userid;
	s += "';";
	std::string result = ExecuteQuery(s, "*");
	std::vector<std::string> v{ explode(result, '\n') };
	Order _n = {
		v[0],
		v[1],
		stoi(v[2]),
		v[3],
		stoi(v[4])
	};
	return _n;
}

std::vector<Order> manager::getAllOrders()
{
	std::string result = ExecuteQuery("SELECT orderid FROM orders;", "orderid");
	std::vector<std::string> v{ explode(result, '\n') };
	std::vector<Order> _orders;
	for (auto s : v)
		_orders.push_back(getOrder(s));
	return _orders;
}

bool manager::doesOrderExist(std::string orderid)
{
	for each (Order o in getAllOrders())
		if (o.order_id == orderid)
			return true;
	
	return false;
}

bool manager::isOrderRedeemed(std::string orderid) 
{
	for each (Order o in getAllOrders())
		if (o.order_id == orderid && o.uid != NULL)
			return true;
	return false;
}

bool manager::isOrderPaidFor(std::string orderid)
{
	Order o = getOrder(orderid);
	if (o.order_paid_for == "null")
		return false;
	return true;
}

std::string manager::getHwid(std::string uid)
{
	std::string s = "SELECT hwid FROM users WHERE uid='";
	s += uid;
	s += "';";
	return ExecuteQuery(s, "hwid");
}

bool manager::doesUsernameExist(std::string username)
{
	std::string s = "SELECT uid FROM users WHERE username='";
	s += username;
	s += "';";
	if (ExecuteQuery(s, "uid") != "")
		return true;
	return false;
}

int manager::getUid(std::string username)
{
	if (!doesUsernameExist(username)) return DBINTERROR;
	std::string s = "SELECT uid FROM users WHERE username='";
	s += username;
	s += "';";
	return stoi(ExecuteQuery(s, "uid"));
}

void manager::addOrder(Order info)
{
	std::string s = "INSERT INTO orders (orderid, orderdate, accessLevel, order_paid_for) VALUES ('";
	s += info.order_id + "', '";
	s += info.order_date + "', ";
	s += info.accessLevel + ", ";
	s += info.order_paid_for + ");";
	ExecuteNonQuery(s);

	return;
}

void manager::redeemOrder(std::string orderid, std::string username)
{
	std::string s = "UPDATE orders SET userid=";
	s += getUid(username) + " WHERE orderid='" + orderid + "';";
	ExecuteNonQuery(s);
	return;
}