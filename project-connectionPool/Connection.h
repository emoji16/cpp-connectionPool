#pragma once
#include<ctime>
#include<string>
#include<mysql.h>

using namespace std;


class Connection {
public:
	Connection();
	~Connection();
	bool connect(string ip, unsigned short port,string username, string password, string dbname);
	bool update(string sql);
	MYSQL_RES* query(string sql);
	void refreshAliveTime();
	clock_t getAliveDuration() const;
private:
	MYSQL* _conn;
	clock_t _alivetime;
};