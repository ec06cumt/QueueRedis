#include "JobMsg.hpp"

CJobMsg::CJobMsg() :m_uiMsgID(0), m_stCreateTime(0)
{

}

CJobMsg::~CJobMsg()
{

}

void CJobMsg::InitMsg(const unsigned int uiPacketID)
{
	m_uiMsgID = 0;
	m_stCreateTime = 0;
	Init(uiPacketID);
}
