#include "ConnectionPool.h"
#include "public.h"

ConnectionPool* ConnectionPool::getConnectionPool() 
{
	static ConnectionPool pool;
	return &pool;
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
	}
	return true;
}