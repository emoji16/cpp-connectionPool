#pragma once
#include<iostream>
#include<queue>
#include "Connection.h"
#include<mutex>
using namespace std;

class ConnectionPool {
public:
	~ConnectionPool();
	static ConnectionPool* getConnectionPool();//tip1：单例模式，注意写法
private:
	ConnectionPool();
	bool loadConfigFile();

	string _ip;//tip2:mysql配置
	unsigned short _port;
	string _username;
	string _password;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;

	queue<Connection*> _connectionQueue;//tip3:线程安全
	mutex _queueMutex;
};