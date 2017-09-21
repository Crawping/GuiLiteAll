#include "../core_include/api.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "../core_include/database.h"
#include "../extern_include/sqlite3.h"

//#define DATA_BASE_PATH	"/data/monitor.db"
#define DATA_BASE_PATH	":memory:"

static char sql_create_real_data_table[] =
		"create table table_real_data("
		"TIME INTEGER,"
		"HR INTEGER,"
		"SPO2 INTEGER,"
		"RR INTEGER,"
		"PR INTEGER,"
		"TEMP INTEGER,"
		"NIBP_SYS INTEGER,"
		"NIBP_DIA INTEGER,"
		"NIBP_MEAN INTEGER,"
		"YZ_HR INTEGER,"
		"YZ_RR INTEGER)";
static char sql_create_limit_cfg_table[] =
		"create table table_limit_cfg("
		"ID INTEGER PRIMARY KEY,"
		"LOW_LIMIT_ADUT INTEGER,"
		"LOW_LIMIT_PET INTEGER,"
		"LOW_LIMIT_NEO INTEGER,"
		"HIGH_LIMIT_ADUT INTEGER,"
		"HIGH_LIMIT_PET INTEGER,"
		"HIGH_LIMIT_NEO INTEGER,"
		"LOW_ALARM_ID	INTEGER,"
		"HIGH_ALARM_ID	INTEGER,"
		"DIGIT INTEGER)";
static char sql_create_sys_settings_table[] =
		"create table table_sys_settings("
		"ID INTEGER PRIMARY KEY,"
		"DATA INTEGER)";

int c_database::m_real_data_rd_cnt;
int c_database::m_real_data_rd_len;
bool c_database::m_rd_cfg_ok;

int c_database::sql_callback_read_real_data(void *arg, int nr, char **values, char **names)
{
	VALUE_SET* data_set = (VALUE_SET*)arg;
	int i = 0;
	if((m_real_data_rd_cnt < m_real_data_rd_len) && data_set)
	{
		data_set[m_real_data_rd_cnt].time = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].hr = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].spo2 = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].rr = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].pr = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].temp = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].nibp_sys = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].nibp_dia = atoi(values[i++]);
		data_set[m_real_data_rd_cnt].nibp_mean = atoi(values[i++]);
		if(i > nr)
		{
			ASSERT(FALSE);
		}
	}
	//printf("%s,%s,%s,%s,%s,%s\n",values[0],values[1],values[2],values[3],values[4],values[5]);
	//fflush(stdout);
	m_real_data_rd_cnt++;
	return 0;
}

int c_database::sql_callback_read_limit_cfg(void *arg, int nr, char **values, char **names)
{
	LIMIT_CONFIG_ITEM* cfg = (LIMIT_CONFIG_ITEM*)arg;
	int i = 0;
	if(cfg)
	{
		cfg->id = atoi(values[i++]);
		cfg->low_limit[0] = atoi(values[i++]);
		cfg->low_limit[1] = atoi(values[i++]);
		cfg->low_limit[2] = atoi(values[i++]);
		cfg->high_limit[0] = atoi(values[i++]);
		cfg->high_limit[1] = atoi(values[i++]);
		cfg->high_limit[2] = atoi(values[i++]);
		cfg->low_alarm_id = atoi(values[i++]);
		cfg->high_alarm_id = atoi(values[i++]);
		cfg->digit = atoi(values[i++]);
	}
	if(i > nr)
	{
		ASSERT(FALSE);
	}
	//printf("%s,%s,%s,%s,%s,%s\n",values[0],values[1],values[2],values[3],values[4],values[5]);
	//fflush(stdout);
	m_rd_cfg_ok = true;
	return 0;
}

int c_database::sql_callback_read_sys_settings(void *arg, int nr, char **values, char **names)
{
	CONFIG_ITEM* cfg = (CONFIG_ITEM*)arg;
	int i = 0;
	if(cfg)
	{
		cfg->id = atoi(values[i++]);
		cfg->data = atoi(values[i++]);
	}
	if(i > nr)
	{
		ASSERT(FALSE);
	}
	//printf("%s,%s,%s,%s,%s,%s\n",values[0],values[1],values[2],values[3],values[4],values[5]);
	//fflush(stdout);
	m_rd_cfg_ok = true;
	return 0;
}

c_database::c_database()
{
	m_db_monitor = NULL;
}

c_database::~c_database()
{
	if(m_db_monitor)
	{
		sqlite3_close(m_db_monitor);
	}
}

int c_database::read(long start_time, long end_time, VALUE_SET* data_set, int len)
{
	char sql_buf[256];
	if(start_time > end_time)
	{
		ASSERT(FALSE);
	}
	m_real_data_rd_cnt = 0;
	m_real_data_rd_len = len;
	memset(sql_buf, 0, sizeof(sql_buf));
	sprintf(sql_buf,
			"select * from table_real_data where TIME between %lu and %lu",
			start_time,
			end_time);
	if(sqlite3_exec(m_db_monitor, sql_buf, sql_callback_read_real_data, data_set, NULL))
	{
		ASSERT(FALSE);
	}
	return MIN(m_real_data_rd_cnt, m_real_data_rd_len);
}

int c_database::write(VALUE_SET &data_set)
{
	//printf("wr time:%d\n",timer);
	//fflush(stdout);
	char sql_buf[256];
    memset(sql_buf, 0, sizeof(sql_buf));
    sprintf(sql_buf,
    		"insert into table_real_data values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
    		data_set.time,
    		data_set.hr,
    		data_set.spo2,
    		data_set.rr,
    		data_set.pr,
    		data_set.temp,
    		data_set.nibp_sys,
    		data_set.nibp_dia,
    		data_set.nibp_mean);
	if(sqlite3_exec(m_db_monitor, sql_buf, NULL, NULL, NULL))
	{
		ASSERT(FALSE);
	}
	return 0;
}

int c_database::display_all()
{
	if(sqlite3_exec(m_db_monitor, "select * from table_real_data", sql_callback_read_real_data, NULL, NULL))
	{
		ASSERT(FALSE);
	}
	return 0;
}

bool c_database::read(int limit_cfg_id, LIMIT_CONFIG_ITEM* data_set)
{
	m_rd_cfg_ok = false;
	char sql_buf[256];
	memset(sql_buf, 0, sizeof(sql_buf));
	sprintf(sql_buf,
			"select * from table_limit_cfg where ID = %d",
			limit_cfg_id);
	int ret = sqlite3_exec(m_db_monitor, sql_buf, sql_callback_read_limit_cfg, data_set, NULL);
	if(ret != 0)
	{
		ASSERT(FALSE);
	}
	if(limit_cfg_id != data_set->id)
	{
		ASSERT(FALSE);
	}
	return m_rd_cfg_ok;
}

int c_database::modify(int limit_cfg_id, LIMIT_CONFIG_ITEM* data_set)
{
	char sql_buf[256];
	memset(sql_buf, 0, sizeof(sql_buf));
	sprintf(sql_buf,
			"update table_limit_cfg set "
			"LOW_LIMIT_ADUT = %d,"
			"LOW_LIMIT_PET = %d,"
			"LOW_LIMIT_NEO = %d,"
			"HIGH_LIMIT_ADUT = %d,"
			"HIGH_LIMIT_PET = %d,"
			"HIGH_LIMIT_NEO = %d,"
			"LOW_ALARM_ID = %d,"
			"HIGH_ALARM_ID = %d,"
			"DIGIT = %d "
			"where ID = %d",
			data_set->low_limit[0],
			data_set->low_limit[1],
			data_set->low_limit[2],
			data_set->high_limit[0],
			data_set->high_limit[1],
			data_set->high_limit[2],
			data_set->low_alarm_id,
			data_set->high_alarm_id,
			data_set->digit,
			data_set->id);
	int ret = sqlite3_exec(m_db_monitor, sql_buf, NULL, NULL, NULL);
	if(ret)
	{
		ASSERT(FALSE);
	}
	return 0;
}

bool c_database::read(int sys_settings_id, CONFIG_ITEM* data_set)
{
	m_rd_cfg_ok = false;
	char sql_buf[256];
	memset(sql_buf, 0, sizeof(sql_buf));
	sprintf(sql_buf,
			"select * from table_sys_settings where ID = %d",
			sys_settings_id);
	int ret = sqlite3_exec(m_db_monitor, sql_buf, sql_callback_read_sys_settings, data_set, NULL);
	if(ret != 0)
	{
		ASSERT(FALSE);
	}
	if(sys_settings_id != data_set->id)
	{
		ASSERT(FALSE);
	}
	return m_rd_cfg_ok;
}

int c_database::modify(int sys_settings_id, CONFIG_ITEM* data_set)
{
	char sql_buf[256];
	memset(sql_buf, 0, sizeof(sql_buf));
	sprintf(sql_buf,
			"update table_sys_settings set "
			"DATA = %d "
			"where ID = %d",
			data_set->data,
			data_set->id);
	int ret = sqlite3_exec(m_db_monitor, sql_buf, NULL, NULL, NULL);
	if(ret)
	{
		ASSERT(FALSE);
	}
	return 0;
}

int c_database::init_limit_cfg(LIMIT_CONFIG_ITEM* cfg, int item_cnt)
{
	int i = 0;
	char sql_buf[256];
	if(cfg == NULL)
	{
		return -1;
	}
	int ret = sqlite3_exec(m_db_monitor,sql_create_limit_cfg_table, NULL, NULL, NULL);
	if(0 == ret)
	{//create limit_cfg table,initialise the configure
		for(;i < item_cnt; i++)
		{
			memset(sql_buf, 0, sizeof(sql_buf));
			sprintf(sql_buf,
					"insert into table_limit_cfg values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
					cfg[i].id,
					cfg[i].low_limit[0],
					cfg[i].low_limit[1],
					cfg[i].low_limit[2],
					cfg[i].high_limit[0],
					cfg[i].high_limit[1],
					cfg[i].high_limit[2],
					cfg[i].low_alarm_id,
					cfg[i].high_alarm_id,
					cfg[i].digit);
			if(sqlite3_exec(m_db_monitor, sql_buf, NULL, NULL, NULL))
			{
				ASSERT(FALSE);
			}
		}
	}
	else if(1 == ret)
	{//open limit_cfg table,load the configure
		for(i = 0; i < item_cnt; i++)
		{
			if(false == read(cfg[i].id, &cfg[i]))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

int c_database::init_sys_settings(CONFIG_ITEM* sys_settings, int item_cnt)
{
	int i = 0;
	char sql_buf[256];
	if(sys_settings == NULL)
	{
		return -1;
	}
	int ret = sqlite3_exec(m_db_monitor,sql_create_sys_settings_table, NULL, NULL, NULL);
	if(0 == ret)
	{//create limit_cfg table,initialise the configure
		for(;i < item_cnt; i++)
		{
			memset(sql_buf, 0, sizeof(sql_buf));
			sprintf(sql_buf,
					"insert into table_sys_settings values(%d,%d)",
					sys_settings[i].id,
					sys_settings[i].data);
			if(sqlite3_exec(m_db_monitor, sql_buf, NULL, NULL, NULL))
			{
				ASSERT(FALSE);
			}
		}
	}
	else if(1 == ret)
	{//open limit_cfg table,load the configure
		for(i = 0; i < item_cnt; i++)
		{
			if(false == read(sys_settings[i].id, &sys_settings[i]))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

int c_database::init()
{
	int ret = -1;
	if(sqlite3_open(DATA_BASE_PATH, &m_db_monitor))
	{
		ASSERT(FALSE);
	}
	ret = sqlite3_exec(m_db_monitor,sql_create_real_data_table, NULL, NULL, NULL);
	if(ret !=0 && ret !=1)
	{
		ASSERT(FALSE);
	}
	return 0;
}
