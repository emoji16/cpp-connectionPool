#pragma once
#include<iostream>
#include<queue>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include "Connection.h"
using namespace std;

class ConnectionPool 
{
public:
	static ConnectionPool* getConnectionPool();// tip1：单例模式，注意写法
	shared_ptr<Connection> getConnection();// tip6？:考虑类型，如果返回Connection*类型，会自动调用析构；使用智能指针，重定义删除机制
	int getQueueSize();
private:
	ConnectionPool(); //tip2:构造函数私有且需要定义:load配置信息+初始化Connectionqueue+开启生产者线程
	bool loadConfigFile();

	// 运行在独立的线程中，专门负责生产新连接
	void produceConnectionTask();//生产者模型
	void scannnerConnectionTask();//不断扫描超时未用的connection

	string _ip;// tip3:从文件读取mysql配置
	unsigned short _port;
	string _dbname;
	string _username;
	string _password;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;

	queue<Connection*> _connectionQueue;// tip4？:生产者消费者模型实现？？
	mutex _queueMutex;//tip:保护线程安全！
	atomic_int _connectionCnt;// tip5:多线程互斥类型，记录总创建的connection数不能超过_maxSize，不只是线程池中的数量
	condition_variable cv;// 设置条件变量，用于连接消费者线程和生产者线程之间的通信
};