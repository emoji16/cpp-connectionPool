#pragma once
#include<string>
#include<mysql.h>

using namespace std;


class Connection {
public:
	Connection();
	~Connection();
	bool connect(string ip, unsigned short port,string user, string password, string dbname);
	bool update(string sql);
	MYSQL_RES* query(string sql);
private:
	MYSQL* _conn;
};