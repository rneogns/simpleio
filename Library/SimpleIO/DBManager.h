#pragma once

class MySQL;

class DBManager
{
private:
	unordered_map<int, MySQL* > dbHandles;	// < DB Handle Index, MySQL * >

public:
	DBManager();
	virtual ~DBManager();

	bool Open(int idx, const char* ip, unsigned int port, const char* dbname, const char* id, const char* passwd);
	void Close();

	MySQL* GetDBHandle(int idx);

};
