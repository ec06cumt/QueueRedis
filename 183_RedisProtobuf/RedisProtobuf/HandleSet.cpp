#include "HandleSet.hpp"

CHandleSet::CHandleSet()
{
	//memset(m_arrPHandle,0,sizeof(m_arrPHandle));
	m_mpHandle.clear();
}

CHandle* CHandleSet::GetHandle(const unsigned int iHandleID)
{
	//printf("iHandleID:%u\n",iHandleID);
	std::map<unsigned int,CHandle*>::iterator it = m_mpHandle.find(iHandleID);
	if(it == m_mpHandle.end())
	{
		//printf("Get Handle NULL\n");
		return NULL;
	}
	//printf("Get Handle success\n");
	return it->second;
}

int CHandleSet::RegisterHandler(const unsigned int iHandleID,CHandle* pHandle)
{
	if(!pHandle)
	{
		return -1;
	}
	m_mpHandle.insert(std::make_pair(iHandleID,pHandle));
    return 0;
}

int CHandleSet::Init(int idx)
{
	return 0;
}

