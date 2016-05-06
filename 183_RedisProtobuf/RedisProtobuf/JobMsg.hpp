/*
 * Create on 2015/04/05 消息包装类
 */

#ifndef _JOBMSG_H_
#define _JOBMSG_H_

#include "RedisPacket.hpp"
#include <time.h>

class CJobMsg:public CPacket
{
public:
	CJobMsg();
	~CJobMsg();

	void InitMsg(const unsigned int uiPacketID);

	void InitEncode()
	{
		m_pData = m_pDataReal;
		InitPacketDB();
	}

	void SetMsgID(unsigned int uiMsgID)
	{
		m_uiMsgID = uiMsgID;
	}

	unsigned int GetMsgID()
	{
		return m_uiMsgID;
	}

	void SetCreateTime(time_t dw)
	{
		m_stCreateTime = dw;
	}

	time_t GetCreateTime()
	{
		return m_stCreateTime;
	}

	int GetRealSize()
	{
		int iSize =  sizeof(*this) - sizeof(m_pDataReal) + m_iLen;
		return iSize;
	}

	bool CheckMsgSize()
	{
		unsigned long iSize = sizeof(*this);

		unsigned long ioffset = (unsigned long)((unsigned long)(this->m_pDataReal) - (unsigned long)(this));
		if (iSize - ioffset == sizeof(this->m_pDataReal))
		{
			return true;
		}
		return false;
	}

	void SetRealInfo()
	{
		m_pBuffer = m_pDataReal;
		m_pData = m_pDataReal;
	}

private:
	unsigned int m_uiMsgID;
	time_t  m_stCreateTime;

	char m_pDataReal[MAX_PACKETDB_SIZE];
};

#endif
