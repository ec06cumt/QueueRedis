#include "RedisProcMgr.hpp"


CProcMgr::~CProcMgr()
{
	if (!m_mapProc.empty())
	{
		m_mapProc.clear();
	}
}

void CProcMgr::Register(const unsigned int packetID, const LPDBJOB_PROC proc)
{
	m_mapProc.insert(std::map<unsigned int, LPDBJOB_PROC>::value_type(packetID, (LPDBJOB_PROC)proc));
}

LPDBJOB_PROC CProcMgr::FindProc(const unsigned int packetID)
{
	std::map<unsigned int, LPDBJOB_PROC>::iterator iter = m_mapProc.find(packetID);

	if (iter != m_mapProc.end())
	{
		return iter->second;
	}
	return NULL;
}
