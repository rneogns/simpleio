#include "Precompiled.h"
#include "MySQLConnector.h"
#include "StringUtil.h"
#include "Log.h"
#include <errmsg.h>

MySQL::MySQL()
	: mysql(nullptr)
	, result_set(nullptr)
	, query_len(0)
	, num_of_rows(0)
	, num_of_fields(0)
	, db_errno(0)
	, fields_lengths(nullptr)
	, curr_read_field(0)
{
}

MySQL::~MySQL()
{
	Close();
}

bool MySQL::Initialize()
{
	if (mysql)
		mysql_close(mysql);

	mysql = mysql_init(nullptr);

	if (!mysql)
	{
		return false;
	}

	// Use the compressed client/server protocol. 
	mysql_options(mysql, MYSQL_OPT_COMPRESS, 0);
	// Read options from the named group from my.cnf or the file specified with MYSQL_READ_DEFAULT_FILE.
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "UseSQL");
	// The name of the character set to use as the default character set.
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	// Enable or disable automatic reconnection to the server if the connection is found to have been lost.Reconnect is off by default; this option provides a way to set reconnection behavior explicitly.
	bool reconnect = true;
	mysql_options(mysql, MYSQL_OPT_RECONNECT, (char*)&reconnect);

	return true;
}

bool MySQL::Open(const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd)
{
	if (!Initialize())
	{
		return false;
	}

	if (!mysql_real_connect(mysql, WcsToMbcs(ip), WcsToMbcs(id), WcsToMbcs(passwd), WcsToMbcs(dbname), port, nullptr, CLIENT_INTERACTIVE))
	{
		db_errno = mysql_errno(mysql);
		return false;
	}

	return true;
}

void MySQL::Close()
{
	ClearResultSet();

	mysql_close(mysql);
	mysql = nullptr;
}

bool MySQL::ExecuteQuery(const wchar_t* format, ...)
{
	// �����μ� ������ �ϼ��ϰ�
	va_list args;
	va_start(args, format);
	wvsprintf(query, format, args);
	va_end(args);

	// ���� ����� �Ѿ���� Ȯ���Ѵ�
	query_len = (unsigned long)wcslen(query);
	if (query_len >= MAX_QUERY_LEN)
	{
		LOG_CORE(L"Query length over. check the query (%u:%s)", query_len, query);
		return true;
	}

	// ���� ����� ���� ������� �����ϰ�
	ClearResultSet();

	bool query_result = false;
	// ������ �����Ѵ�. �������� ��� ��� ��õ� �غ���
	for (int i = 0; i < RETRY_COUNT; ++i)
	{
		query_result = DoQuery();
		if (query_result == true)
		{
			break;
		}
		else
		{
			// DB�۾��� ��õ� �ص� ������ �� ������. �α׸� ���, ���Ŀ��� ������ �������� �ϴ°� ���ڴ�.
			if (i == RETRY_COUNT - 1)
			{
				LOG_CORE(L"db job failed");
				return false;
			}
		}
	}

	// ��������� MySQL�κ��� ������ ����¿� �����Ѵ�
	result_set = mysql_store_result(mysql);

	num_of_rows = 0;
	num_of_fields = 0;

	if (result_set)
	{
		num_of_rows = mysql_num_rows(result_set);
		num_of_fields = mysql_num_fields(result_set);
	}

	return true;
}

bool MySQL::DoQuery()
{
	// ������ �����ϰ�, ���н� ������ȣ�� �����ϰ� �����Ѵ�
	int ret = mysql_real_query(mysql, WcsToMbcs(query), query_len);
	if (ret != 0)
	{
		db_errno = mysql_errno(mysql);

		// �Ʒ��� ��쿡�� ������ �翬���� �õ��غ���
		if (db_errno == CR_SERVER_GONE_ERROR || db_errno == CR_SERVER_LOST
			|| db_errno == CR_UNKNOWN_HOST || db_errno == CR_CONN_HOST_ERROR)
		{
			LOG_CORE(L"mysql connection lost (error: %d), try reconnect", db_errno);

			// MYSQL_OPT_RECONNECT �ɼ��� ���������Ƿ� mysql_ping���� �翬���� �õ��غ���
			if (mysql_ping(mysql) != 0)
			{
				// �����ϸ� �ٽ� ������ ����
				if (!Open(MbcsToWcs(mysql->host), mysql->port, MbcsToWcs(mysql->db), MbcsToWcs(mysql->user), MbcsToWcs(mysql->passwd)))
					return false;
			}
		}

		return false;
	}

	return true;
}

bool MySQL::Fetch()
{
	// ������� �� ���� fetch �ؿ´�

	if (!result_set)
		return false;

	// ������� ���� row�� �����Ѵ�. ������ null����
	fetched_row = mysql_fetch_row(result_set);
	if (!fetched_row)
		return false;

	// ���� row�� �÷�(������)�� ���̸� �����Ѵ�. 
	fields_lengths = mysql_fetch_lengths(result_set);

	// ���� �ʵ��ȣ�� �ʱ�ȭ���ش�
	curr_read_field = 0;

	return true;
}

void MySQL::ClearResultSet()
{
	if (result_set)
	{
		mysql_free_result(result_set);
		result_set = nullptr;
	}

	fields_lengths = nullptr;
	num_of_rows = 0;
	num_of_fields = 0;
	curr_read_field = 0;
}

unsigned long long MySQL::GetAffectedRowCount()
{
	return mysql_affected_rows(mysql);
}

unsigned long long MySQL::GetNumOfRows()
{
	return num_of_rows;
}

unsigned long MySQL::GetNumOfFields()
{
	return num_of_fields;
}

bool MySQL::GetByte(char& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetByte(unsigned char& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetShort(short& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetShort(unsigned short& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt(int& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt(unsigned int& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt64(long long& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = _atoi64(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt64(unsigned long long& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = _atoi64(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetFloat(float& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = (float)atof(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetDouble(double& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atof(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetBinary(char* buffer, int size)
{
	memset(buffer, 0, size);
	memcpy(buffer, fetched_row[curr_read_field], fields_lengths[curr_read_field]);
	curr_read_field++;

	return true;
}

bool MySQL::GetString(char* buffer, int size)
{
	strncpy(buffer, fetched_row[curr_read_field++], size);

	return true;
}

bool MySQL::GetString(wchar_t* buffer, int size)
{
	char strbuf[8192] = { 0, };
	strncpy(strbuf, fetched_row[curr_read_field++], sizeof(strbuf));
	wcsncpy(buffer, MbcsToWcs(strbuf), size);

	return true;
}