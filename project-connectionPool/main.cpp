#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <mysql.h>
#include <thread>
#include "Connection.h"
#include "ConnectionPool.h"

using namespace std;

int main() {
	//���߳�ѹ������
	/*clock_t begin = clock();
	for (int i = 0; i < 1000; ++i) {
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
		conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
		conn.update(sql);
	}
	clock_t end = clock();
	cout << "δʹ�����ӳ���ʱ��" << (end - begin) << "ms" << endl;
	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	begin = clock();
	for (int i = 0; i < 1000; ++i) {
		shared_ptr<Connection> p = cp->getConnection() ;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
		p->update(sql);
	}
	end = clock();
	cout << "ʹ�����ӳ���ʱ��" << (end - begin) << "ms" << endl;*/
	//���߳�ѹ������
	/*Connection con;
	con.connect("127.0.0.1", 3306, "root", "123456", "chat");
	clock_t begin = clock();
	thread t1([]() {
			for (int i = 0; i < 250; ++i) {
				Connection conn;
				char sql[1024] = { 0 };
				sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
				conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
				conn.update(sql);
			}
	});
	thread t2([]() {
		for (int i = 0; i < 250; ++i) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
		});
	thread t3([]() {
		for (int i = 0; i < 250; ++i) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
		});
	thread t4([]() {
		for (int i = 0; i < 250; ++i) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
		});
	t1.join();//join?
	t2.join();//join?
	t3.join();//join?
	t4.join();//join?
	clock_t end = clock();
	cout << "δʹ�����ӳ���ʱ��" << (end - begin) << "ms" << endl;*/
	clock_t begin = clock();
	thread t1([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();// ע�����Ҫд���߳����棬���캯����д��ȷ���˵���ģʽ
		for (int i = 0; i < 250; ++i) {
			shared_ptr<Connection> p = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			p->update(sql);
		}
		});
	thread t2([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i) {
			shared_ptr<Connection> p = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			p->update(sql);
		}
		});
	thread t3([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i) {
			shared_ptr<Connection> p = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			p->update(sql);
		}
		});
	thread t4([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i) {
			shared_ptr<Connection> p = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
			p->update(sql);
		}
		});
	t1.join();//join?
	t2.join();//join?
	t3.join();//join?
	t4.join();//join?
	clock_t end = clock();
	cout << "ʹ�����ӳ���ʱ��" << (end - begin) << "ms" << endl;
	return 0;
}
