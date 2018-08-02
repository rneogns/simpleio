#pragma once

#include <mysql.h>
#include <boost/lexical_cast.hpp>

// reference: http://dev.mysql.com/doc/refman/5.7/en/c-api.html

#define MAX_QUERY_LEN 16384
#define RETRY_COUNT 5

class MySQL
{
private:
	MYSQL* mysql;						// MySQL �ڵ�
	MYSQL_RES* result_set;				// ������� ����� ������
	MYSQL_ROW fetched_row;				// ������� �ϳ��� row�� ����Ű�� ������
	wchar_t query[MAX_QUERY_LEN];		// ���� �� ���� ����
	unsigned long query_len;			// ���� �� ���� ����
	unsigned long long num_of_rows;		// ��� �� ��
	unsigned int num_of_fields;			// ����� �÷� ��
	unsigned int db_errno;				// �߻��� ����
	unsigned long* fields_lengths;		// ����������� �� �ʵ��� ���̸� ����Ű�� ������
	unsigned int curr_read_field;		// ��� ROW���� �о� �� �÷��� �ε���

public:
	MySQL();
	~MySQL();

	bool Open(const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd);
	void Close();
	bool ExecuteQuery(const wchar_t* format, ...);
	bool Fetch();
	void ClearResultSet();	

	unsigned long long GetAffectedRowCount();
	unsigned long long GetNumOfRows();
	unsigned long GetNumOfFields();

	bool GetByte(char& o);
	bool GetByte(unsigned char& o);
	bool GetShort(short& o);
	bool GetShort(unsigned short& o);
	bool GetInt(int& o);
	bool GetInt(unsigned int& o);
	bool GetInt64(long long& o);
	bool GetInt64(unsigned long long& o);
	bool GetFloat(float& o);
	bool GetDouble(double& o);
	bool GetBinary(char* buffer, int size);
	bool GetString(char* buffer, int size);
	bool GetString(wchar_t* buffer, int size);

	template <typename T>
	bool Get(T& out)
	{
		if (!result_set || curr_read_field >= num_of_fields)
			return false;

		try
		{
			o = boost::lexical_cast<T>(fetched_row[curr_read_field++]);
		}
		catch (bad_lexical_cast e)
		{
			std::cout << e.what() << std::endl;
		}		
	}

private:
	bool Initialize();
	bool DoQuery();

};