/*
 * Create on 2016-04-02 by liug
 * threadManager.cpp
 * 线程管理器
 */

#ifndef _THREADREDIS_H_
#define _THREADREDIS_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "DBRedis.hpp"
#include "CCodeQueueA.hpp"
#include "JobMsg.hpp"
#include "RedisProcMgr.hpp"
#include "PacketID.hpp"
#include "HandleRedisSet.hpp"
#include "CodeQueue.hpp"
#include <list>

#include "ProtoEngine.hpp"
#include "RedisProtoProcMgr.hpp"

using namespace ServerLib;
typedef struct STDBPACKINFO
{
	char* m_stPacket;
	int m_iPacketLen;
}STDBPACKETINFO,*PDBPACKETINFO;

//#define REGISTER_JOB_PROC(X) m_ProtoProcMgr.Register(##X,CThreadRedis::_Proc_##X)
#define FUNC_JOB(X) (CThreadRedis::_Proc_##X)
#define DECLARE_JOB_PROC(X) static void _Proc_##X(DBClientRedis* pRedisClient,GameMsg* pGameMsg)
#define DEFINE_JOB_PROC(X) void CThreadRedis::_Proc_##X(DBClientRedis* pRedisClient,GameMsg* pGameMsg)

#define GETVALUE2(X) memcpy(&X,pBuffer,sizeof(X)); pBuffer += sizeof(X)
#define GETDATA2(X,LEN) memcpy(X,pBuffer,LEN); pBuffer += LEN

class CThreadRedis
{
public:
	CThreadRedis();
	~CThreadRedis();

	int Init(int iThreadIndex);
	char GetEndFlag(){ return m_bEndThread;};
	void SetEndFlag(char bEnded){ m_bEndThread = bEnded; };
	void SetThreadIndex(int iThreadIndex) {m_iThreadIdx = iThreadIndex;};
	int  GetThreadIdx(){	return m_iThreadIdx;};


	void Process();
	void ProcessAll();
	void MyProcess();
	void MyProcessAll();

	int PushCode(char* pMsg,int iLen);
	int PopCode(char* pMsg,int& iLen);

	void SetFailedFlag(int* pFailedCount);
private:
	int CreateThread();

	void Run();

	//static void GetKeyValue(DBClientRedis* pRedisClient,GameMsg* pMsg);
//	DECLARE_JOB_PROC(PROTO_MSGID_GETINFO_REQUEST);

	CHandleRedisSet m_HandRedisSet;
	int ProcessMsg();
	int EncodeAndSendMsg();
private:
	CCodeQueueA* m_pMyCodeQueue;
	CCodeQueue* m_pCodeQueue;

	int m_iThreadIdx;			//线程ID下标
	pthread_t m_hThread;
	pthread_attr_t m_stAttr;

	char m_bEndThread;
	unsigned char m_szCodeBuf[MAX_MSGBUFFER_SIZE];
	char m_szMyCodeBuf[MAX_MSGBUFFER_SIZE];

	CProtoEngine m_ProtoEngine;
	GameMsg m_stGameMsg;

//	CProtoProcMgr m_ProtoProcMgr;
	//static DBClientRedis m_RedisClient;

private:
	CCodeQueueA* m_pMyCodeOutQueue;
	unsigned int m_uiMaxIdx;
	int* m_pFailedCount;

};

#endif
