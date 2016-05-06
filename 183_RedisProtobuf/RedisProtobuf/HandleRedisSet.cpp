#include "HandleRedisSet.hpp"

CHandleRedisSet::CHandleRedisSet():m_HandleGet(&m_dbClientRedis)
{
	m_iThreadIdx = 0;
}

int CHandleRedisSet::Init(int idx)
{
	m_iThreadIdx = idx;
	return RegisterAllHandler();
}

int CHandleRedisSet::RegisterAllHandler()
{
	int iRet = 0;
	iRet = RegisterHandler(PROTO_MSGID_GETINFO_REQUEST,&m_HandleGet);
	if(iRet != 0)
	{
		return -1;
	}
	return iRet;
}
