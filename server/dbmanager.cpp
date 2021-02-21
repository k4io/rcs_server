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
	using namespace std;
	out.out(0, "[mysql] Checking connection to database");
	sql::Driver* driver;
	sql::Connection* conn;

	try {
		driver = get_driver_instance();
		conn = driver->connect("tcp://localhost:3306", "root", "gaygay555");
		//conn = driver->connect("tcp://[::1]:3306", "root", "");
		conn->setSchema("smoke");
		//printf("IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED IT WORKED");
		stmt = conn->createStatement();
		return true;
	}
	catch (sql::SQLException& e) {
		out.out(2, "[mysql] Could not connect to database");
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}
}

std::string manager::ExecuteNonQuery(std::string query)
{
	out.out(0, "[mysql] ExecuteNonQuery() called with query: " + query);
	using namespace std;
	try {
		std::string s = "";
		if (!connect())
			return s;
		stmt->execute(query);
		return s;
	}
	catch (sql::SQLException& e) {
		out.out(2, "[mysql] ExecuteNonQuery() Errored with query: " + query);
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}
}

std::string manager::ExecuteQuery(std::string query, std::string column)
{
	out.out(0, "[mysql] ExecuteQuery() called with query: " + query);
	using namespace std;
	try {
	std::string s = "";
	if (!connect())
		return s;
	res = stmt->executeQuery(query); 

	while (res->next())
		s += res->getString(column) + "\n";

	out.out(0, "[mysql] ExecuteQuery() called with query: " + query + " returned: " + s);
	return s;
	}
	catch (sql::SQLException& e) {
		//throw e;
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}
}

int manager::ExecuteQueryInt(std::string query, std::string column)
{
	out.out(0, "[mysql] ExecuteQueryInt() called with query: " + query);
	using namespace std;
	try {
		std::string s = "";
		if (!connect())
			return -69;
		res = stmt->executeQuery(query);
		int i = DBINTERROR;
		while (res->next())
			i = res->getInt(column);
		out.out(0, "[mysql] ExecuteQueryInt() called with query: " + query + " returned: " + std::to_string(i));
		return i;
	}
	catch (sql::SQLException& e) {
		out.out(2, "[mysql] ExecuteQueryInt() Errored with query: " + query);
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}
}

Order manager::getOrder(std::string orderid)
{
	std::string s = "SELECT * FROM orders WHERE orderid='";
	s += orderid;
	s += "';";
	//std::string result = ExecuteQuery(s, "*");

	int al;
	int paid;
	int id;
	std::string orderdate = "";

	using namespace std;
	try {
		if (!connect())
			throw;
		res = stmt->executeQuery(s);

		while (res->next())
		{
			orderdate = res->getInt("orderdate");
			al = res->getInt("accessLevel");
			paid = res->getInt("order_paid_for");
			id = res->getInt("uid");
		}
	}
	catch (sql::SQLException& e) {
		//throw e;
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}

	//std::vector<std::string> v{ explode(result, '\n') };
	Order _n = {
		orderid,
		orderdate,
		al,
		paid,
		id
	};
	return _n;
}

Order manager::getOrderuid(int userid)
{
	std::string s = "SELECT * FROM orders WHERE uid=";
	s += std::to_string(userid);
	s += ";";

	std::string orderid = "";
	std::string orderdate = "";
	int al;
	int paid;
	int id;

	using namespace std;
	try {
		if (!connect())
			throw;
		res = stmt->executeQuery(s);

		while (res->next())
		{
			orderid = res->getString("orderid");
			orderdate = res->getString("orderdate");
			al = res->getInt("accessLevel");
			paid = res->getInt("order_paid_for");
			id = res->getInt("uid");
		}
	}
	catch (sql::SQLException& e) {
		//throw e;
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		system("pause");
		exit(-1);
	}
	Order _n = {
		orderid,
		orderdate,
		al,
		paid,
		id
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
	if (o.order_paid_for == 0)
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

bool manager::doesUidExist(int uid)
{
	std::string s = "SELECT username FROM users WHERE uid=" + std::to_string(uid) + ";";

	if (ExecuteQuery(s, "uid") != "")
		return true;
	return false;
}

bool manager::doesDuidExist(std::string duid)
{
	std::string s = "SELECT uid FROM users WHERE discorduid='" + duid + "';";

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
	s += info.order_id + "', DATE('";
	s += info.order_date + "'), ";
	s += std::to_string(info.accessLevel) + ", ";
	if(info.order_paid_for == 0)
		s += "0);";
	else
		s += "1);";

	ExecuteNonQuery(s);
	return;
}

void manager::redeemOrder(std::string orderid, std::string username)
{
	std::string s = "UPDATE orders SET uid=";
	s += std::to_string(getUid(username)) + " WHERE orderid='" + orderid + "';";
	ExecuteNonQuery(s);
	return;
}

void manager::addUser(std::string username, std::string pwdhash, int accessLevel, std::string hwid)
{
	std::string s = "INSERT INTO users (username, accessLevel, pwdhash, hwid, creation_date) VALUES ('";
	s += username + "', ";
	s += accessLevel + ", '";
	s += pwdhash + "', '";
	s += hwid + "', ";
	s += "NOW())";

	ExecuteNonQuery(s);
	return;
}

void manager::addUser(std::string username, std::string pwdhash, int accessLevel, std::string hwid, std::string duid)
{
	std::string s = "INSERT INTO users (username, accessLevel, pwdhash, hwid, creation_date, discorduid) VALUES ('";
	s += username + "', " + std::to_string(accessLevel) + ", '";
	s += pwdhash + "', '";
	s += hwid + "', ";
	s += "NOW(), '";
	s += duid + "')";

	ExecuteNonQuery(s);
	return;
}

void manager::removeUser(std::string username)
{
	if (doesUsernameExist(username))
	{
		std::string s = "DELETE FROM users WHERE username='" + username + "';";
		ExecuteNonQuery(s);
		return;
	}
}

void manager::removeOrder(std::string orderid)
{
	if (doesOrderExist(orderid))
	{
		std::string s = "DELETE FROM orders WHERE orderid='" + orderid + "';";
		ExecuteNonQuery(s);
		return;
	}
}

void manager::setHwid(int uid, std::string hwid)
{
	if (doesUidExist(uid)) {
		std::string s = "UPDATE users SET hwid='" + hwid + "' WHERE uid=" + std::to_string(uid) + ";";
		ExecuteNonQuery(s);
		return;
	}
}

int manager::isAccountLocked(std::string username)
{
	std::string s = "SELECT locked FROM users WHERE username='" + username + "';";
	return ExecuteQueryInt(s, "locked");
}

std::string manager::getLockedReason(std::string username)
{
	std::string s = "SELECT lockedReason FROM users WHERE username='" + username + "';";
	std::string result = ExecuteQuery(s, "lockedReason");
	return result;
}

std::string manager::getAllUids()
{	
	return ExecuteQuery("SELECT uid FROM users;", "uid");	
}

std::string manager::getSubDate(int uid)
{
	return ExecuteQuery("SELECT subscriptionStart FROM users WHERE uid='" + std::to_string(uid) + "';", "subscriptionStart");
}

std::string manager::getSubEndDate(int uid)
{
	return ExecuteQuery("SELECT subscriptionEnd FROM users WHERE uid='" + std::to_string(uid) + "';", "subscriptionEnd");
}

void manager::setSubDate(std::string date, int accessLevel, int uid)
{
	ExecuteNonQuery("UPDATE users SET subscriptionStart=DATE(" + date + ") WHERE uid=" + std::to_string(uid) + ";");

	//set end date
	std::string timeindb = getSubDate(uid);

	timeindb = explode(timeindb, ' ')[0];

	struct tm tm;

	time_t start;
	int yy, month, dd, hh, mm, ss;
	const char* zstart = timeindb.c_str();

	sscanf(zstart, "%d-%d-%d", &yy, &month, &dd);


	tm.tm_year = yy - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = dd;
	tm.tm_isdst = -1;


	start = mktime(&tm);
	time_t newtime;
	time_t now = time(0);

	switch (accessLevel)
	{
	case 1: //vip 30d
		newtime = now + (60 * 43200);
		break;
	case 2: //vip+ 30d
		newtime = now + (60 * 43200);
		break;
	case 3: //vip 6m
		newtime = now + (60 * 259200);
		break;
	case 4: //vip+ 6m
		newtime = now + (60 * 259200);
		break;
	}

	struct tm* ntm = gmtime(&newtime);

	int nyy = ntm->tm_year + 1900;
	int nmm = ntm->tm_mon + 1;
	int ndd = ntm->tm_mday;
	std::string s = std::to_string(nyy) + "-" + std::to_string(nmm) + "-" + std::to_string(ndd);

	ExecuteNonQuery("UPDATE users SET subscriptionEnd=DATE('" + s + "') WHERE uid=" + std::to_string(uid) + ";");
	//printf("%o/%o/%o", nyy, nmm, ndd);

	return;
}

std::string manager::getUname(int uid)
{
	return ExecuteQuery("SELECT username FROM users WHERE uid=" + std::to_string(uid) + ";", "username");
}

void manager::expiredUser(int uid)
{
	std::string uname = getUname(uid);
	std::string substart = getSubDate(uid);
	std::string subend = getSubEndDate(uid);
	Order _o = getOrderuid(uid);
	int accessLevel = _o.accessLevel;

	ExecuteNonQuery("INSERT INTO sublog (uid, username, subscriptionStart, subscriptionEnd, orderid, accessLevel) VALUES (" + std::to_string(uid) + ", '" + uname + "', DATE('" + substart + "'), DATE('" + subend + "'), '" + _o.order_id + "', " + std::to_string(accessLevel) + ");");
	ExecuteNonQuery("DELETE FROM users WHERE uid=" + std::to_string(uid) + ";");
}

void manager::unlockAccount(std::string username)
{
	std::string s = "UPDATE users SET locked=0 WHERE username='" + username + "';";
	ExecuteNonQuery(s);
	return;
}