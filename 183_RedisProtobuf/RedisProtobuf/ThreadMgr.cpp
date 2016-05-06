#include "ThreadMgr.hpp"


CThreadMgr::CThreadMgr()
{
//	for(int i= 0;i< MAX_THREAD_NUMBER;i++)
//	{
//		m_arrQuitFlag[i] = 0;
//	}

}
CThreadMgr::~CThreadMgr()
{
	printf("dected CThreadMgr\n");
}

int CThreadMgr::Init()
{
	int iRet = 0;
	for(int i = 0;i< MAX_THREAD_NUMBER;i++)
	{
		printf("idx==%d\n",i);
		iRet = 0;

		iRet = m_arrThread[i].Init(i);

		if(iRet)
		{
			printf("Init Thread Failed\n");
			break;
		}
	}
	return iRet;
}

CThreadRedis* CThreadMgr::GetThreadRedis(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_THREAD_NUMBER)
	{
		return NULL;
	}

	return &m_arrThread[iIndex];
}

int CThreadMgr::PushMsg(unsigned int userID,char* pMsg,int iLen)
{
	int idx = (int)userID % MAX_THREAD_NUMBER;
	return m_arrThread[idx].PushCode(pMsg,iLen);
}

int CThreadMgr::PopJobMsg(int iIndex, char* pJobMsg,int& iLen)
{
	return m_arrThread[iIndex].PopCode(pJobMsg,iLen);
}

void CThreadMgr::SetQuitFlag(int (&arr)[MAX_THREAD_NUMBER])
{
	for(int i = 0;i< MAX_THREAD_NUMBER;i++)
	{
		m_arrThread[i].SetFailedFlag(&arr[i]);
	}

}
//void GetQuitFlag(int& iQuitFlay)
//{
//
//}
