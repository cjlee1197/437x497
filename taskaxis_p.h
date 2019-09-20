#include "taskaxis.h"

#pragma once

class CtmAxisPrivateData{
	public:
		pthread_mutex_t 	m_lock;
		map<AXISID,AxisItem>	m_item_write[CTaskAxis::AXIS_ALL];
		map<AXISID,AxisItem>	m_item_read[CTaskAxis::AXIS_ALL];
		map<AXISID,AxisItem>	m_item_table;
		vector<Exception>		m_exception;
		CXmlNode			m_root;
		CTaskAxis::AXIS		m_axis;
		int					m_rsp;
	public:
		CtmAxisPrivateData():m_rsp(0),m_axis(CTaskAxis::AXIS_ALL){}
		~CtmAxisPrivateData(){}
		
};
