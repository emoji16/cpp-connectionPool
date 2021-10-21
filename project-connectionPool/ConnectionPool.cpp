#define _CRT_SECURE_NO_WARNINGS
#include "ConnectionPool.h"
#include<thread>
#include<functional>
#include "public.h"
#include "Connection.h"

ConnectionPool::ConnectionPool() 
{
	if (!loadConfigFile()) //tip:构造函数调用私有成员函数
	{
		return;
	}
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		_connectionQueue.push(p);
		_connectionCnt++;
		
	}
	// 启动新的生产者线程,作为连接的生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));//使用适配器，thread中需要将成员对象和对象绑定
	produce.detach();//主线程结束后自动结束
	//启动新的定时线程，定时清理超时未使用的connection
	thread scan(std::bind(&ConnectionPool::scannnerConnectionTask, this)); 
	scan.detach();//
}

ConnectionPool* ConnectionPool::getConnectionPool() 
{
	static ConnectionPool pool;
	return &pool;
}

shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex> lock(_queueMutex);
	while(_connectionQueue.empty())//这样实现的可以在被抢断之后再继续进行等待
	{
		if(cv_status::timeout == cv.wait_for(lock, chrono::microseconds(_connectionTimeOut)))//tip:不是sleep，而是
		{
			if (_connectionQueue.empty())
			{
				LOG("获取连接失败");
				return nullptr;
			}
		}
	}
	shared_ptr<Connection> sp(_connectionQueue.front(),
		[&](Connection* pcon) {
			unique_lock<mutex> lock(_queueMutex);
			pcon->refreshAliveTime();
			_connectionQueue.push(pcon);
		});//智能指针,自定义spsh失效时的操作:不再按照默认delete；自动归还线程
		//注意这种特殊写法&相当于传入this
	_connectionQueue.pop();
	cv.notify_all();
	return sp;
}

void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQueue.empty()) //生产者消费者
		{
			cv.wait(lock);//生产者线程进入等待状态
		}
		if (_connectionCnt < _maxSize) {
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			_connectionQueue.push(p);
			++_connectionCnt;
		}
		cv.notify_all();//通知消费者线程可以使用

	}
}

void ConnectionPool::scannnerConnectionTask()
{
	for (;;)
	{
		//使用sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize) {
			Connection* p = _connectionQueue.front();
			if (p->getAliveDuration() >= _maxIdleTime * 1000) {
				_connectionQueue.pop();
				--_connectionCnt;
				delete p;
			}
			else break;
		}
	}
}

bool ConnectionPool::loadConfigFile() 
{
	FILE* pf = fopen("mysql.ini","r");
	if (pf == nullptr) 
	{
		LOG("File mysql.ini doesn't exist.");
		return false;
	}
	while (!feof(pf)) 
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1) 
		{
			continue;
		}
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx -1);
		if (key == "ip") {
			_ip = value;
		}
		else if (key == "port") {
			_port = atoi(value.c_str());
		}
		else if (key == "dbname") {
			_dbname = value;
		}
		else if (key == "username") {
			_username = value;
		}
		else if (key == "password") {
			_password = value;
		}
		else if (key == "initSize") {
			_initSize = atoi(value.c_str());
		}
		else if (key == "maxSize") {
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "ConnectionTimeOut") {
			_connectionTimeOut = atoi(value.c_str());
		}
	}
	return true;
}

int ConnectionPool::getQueueSize()
{
	return _connectionQueue.size();
}