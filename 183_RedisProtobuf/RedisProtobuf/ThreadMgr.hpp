/*
 * Create on 2016-04-02 by liug
 * threadManager.cpp
 * 线程管理器
 */

#ifndef _THREADMGR_H_
#define _THREADMGR_H_

#include "ThreadRedis.hpp"
const int MAX_THREAD_NUMBER = 4;

class CThreadMgr
{
public:
	CThreadMgr();
	~CThreadMgr();

	int Init();

	CThreadRedis* GetThreadRedis(int iIndex);



	int PushMsg(unsigned int userID,char* pMsg,int iLen);

	int PopJobMsg(int iIndex,char* pJobMsg,int& iLen);

	void SetQuitFlag(int (&arr)[MAX_THREAD_NUMBER]);

	//void GetQuitFlag(int& iQuitFlay);
private:
	CThreadRedis m_arrThread[MAX_THREAD_NUMBER];

};

#endif

