#include "../core_include/api.h"
#include "../core_include/alarm_queue.h"
#include <string.h>

//find_phy_node / read_phy_node need mutex--tbd
c_alarm_queue::c_alarm_queue()
{
	m_list_tail= m_list_head = m_cycle_cursor = 0;
}

int c_alarm_queue::get_node_by_cycle(unsigned short &str_id, unsigned short &level)
{
	int ret = -1;
	if(m_list_tail == m_list_head)
	{//empty
		return -1;
	}
	if (m_list_tail == m_cycle_cursor)
	{//recycle
		m_cycle_cursor = m_list_head;
	}
	ret = m_node_list[m_cycle_cursor].id;
	str_id = m_node_list[m_cycle_cursor].str_id;
	level = m_node_list[m_cycle_cursor].level;
	m_cycle_cursor = (m_cycle_cursor + 1) % ALARM_QUEUE_LENGTH;
	return ret;
}

int c_alarm_queue::insert_node(unsigned short alarm_id, short level, unsigned short str_id)
{
	if ((m_list_tail + 1) % ALARM_QUEUE_LENGTH == m_list_head)
	{//full
		return -1;
	}
	if(find_node(alarm_id) == false)
	{
		m_node_list[m_list_tail].id = alarm_id;
		m_node_list[m_list_tail].time_in_sec = get_time_in_second();
		m_node_list[m_list_tail].level = level;
		m_node_list[m_list_tail].str_id = str_id;
		//insert to database  tbd
		m_list_tail = (m_list_tail + 1) % ALARM_QUEUE_LENGTH;
		return 0;
	}
	return -1;
}

int c_alarm_queue::read_node()
{
	if (m_list_tail == m_list_head)
	{//empty
		return -1;
	}
	int ret = m_node_list[m_list_head].id;
	m_list_head = (m_list_head + 1) % ALARM_QUEUE_LENGTH;
	return ret;
}

bool c_alarm_queue::find_node(unsigned short alarm_id)
{
	int tmp_head = m_list_head;
	int ret = -1;
	do
	{
		ret = read_node();
	}while(ret > 0 && ret != alarm_id);
	m_list_head = tmp_head;
	if(ret == alarm_id)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int c_alarm_queue::clear_node(unsigned short alarm_id)
{
	int tmp_head = m_list_head;
	int ret = -1;
	ALARM_NODE node;
	do{
		ret = read_node();
	}while(ret > 0 && ret != alarm_id);

	if(ret == alarm_id)
	{
		int target_index = ((m_list_head - 1) >= 0)?(m_list_head - 1):(ALARM_QUEUE_LENGTH - 1);
		memcpy(&node, &m_node_list[target_index], sizeof(ALARM_NODE));
		memcpy(&m_node_list[target_index], &m_node_list[tmp_head], sizeof(ALARM_NODE));
		memcpy(&m_node_list[tmp_head], &node, sizeof(ALARM_NODE));
		m_list_head = tmp_head;
		read_node();
		return 0;
	}
	else
	{
		m_list_head = tmp_head;
		return -1;
	}
}
