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
	static ConnectionPool* getConnectionPool();// tip1������ģʽ��ע��д��
	shared_ptr<Connection> getConnection();// tip6��:�������ͣ��������Connection*���ͣ����Զ�����������ʹ������ָ�룬�ض���ɾ������
	int getQueueSize();
private:
	ConnectionPool(); //tip2:���캯��˽������Ҫ����:load������Ϣ+��ʼ��Connectionqueue+�����������߳�
	bool loadConfigFile();

	// �����ڶ������߳��У�ר�Ÿ�������������
	void produceConnectionTask();//������ģ��
	void scannnerConnectionTask();//����ɨ�賬ʱδ�õ�connection

	string _ip;// tip3:���ļ���ȡmysql����
	unsigned short _port;
	string _dbname;
	string _username;
	string _password;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;

	queue<Connection*> _connectionQueue;// tip4��:������������ģ��ʵ�֣���
	mutex _queueMutex;//tip:�����̰߳�ȫ��
	atomic_int _connectionCnt;// tip5:���̻߳������ͣ���¼�ܴ�����connection�����ܳ���_maxSize����ֻ���̳߳��е�����
	condition_variable cv;// �����������������������������̺߳��������߳�֮���ͨ��
};