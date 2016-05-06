#include "RedisProtoProcMgr.hpp"

CProtoProcMgr::~CProtoProcMgr()
{
	if (!m_mapProc.empty())
	{
		m_mapProc.clear();
	}
}

void CProtoProcMgr::Register(const unsigned int uiMsgID, const LPJOB_PROC proc)
{
	//CGuardLock guardlock(&m_Mutex);
	m_mapProc.insert(std::map<unsigned int, LPJOB_PROC>::value_type(uiMsgID, (LPJOB_PROC)proc));
}

LPJOB_PROC CProtoProcMgr::FindProc(const unsigned int uiMsgID)
{
	//CGuardLock guardlock(&m_Mutex);
	std::map<unsigned int, LPJOB_PROC>::iterator iter = m_mapProc.find(uiMsgID);

	if (iter != m_mapProc.end())
	{
		return iter->second;
	}
	return NULL;
}

void CProtoProcMgr::ExeMsg(const unsigned int uiMsgID,DBClientRedis* pRedisClient,GameMsg* pGameMsg)
{
	LPJOB_PROC pJob = FindProc(uiMsgID);
	pJob(pRedisClient,pGameMsg);
}

