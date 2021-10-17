#pragma once
#include<iostream>
#include<queue>
#include "Connection.h"
#include<mutex>
using namespace std;

class ConnectionPool {
public:
	~ConnectionPool();
	static ConnectionPool* getConnectionPool();//tip1������ģʽ��ע��д��
private:
	ConnectionPool();
	bool loadConfigFile();

	string _ip;//tip2:mysql����
	unsigned short _port;
	string _username;
	string _password;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;

	queue<Connection*> _connectionQueue;//tip3:�̰߳�ȫ
	mutex _queueMutex;
};