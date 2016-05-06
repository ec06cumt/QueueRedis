/*
 * Create on 2015/04/04 liug
 * 简单的线程管理类，消息中转到各个线程
 */
#ifndef _REDISAPP_H_
#define _REDISAPP_H_

#include "ThreadMgr.hpp"
#include "MsgTransceiver.hpp"

class CRedisApp
{
public:
	CRedisApp();
	~CRedisApp();

    // 信号处理
    static int ms_iAppCmd;
    static void SetAppCmd(int iAppCmd);

    int Init();
	void Run();

	void RecvMsg(int& iRecvCount);
	int  ReceiveAndDispatchMsg();

	int PickThreadMsgAndSend(int& riSendMsgCount);

private:
	CThreadMgr m_ThreadMgr;
	CRedisMsgTransceiver m_stMsgTran;
	char m_szCodeBuf[MAX_MSGBUFFER_SIZE];

	int m_arrQuitFlag[MAX_THREAD_NUMBER];

private:
	void GetQuitFlag(int& iQuitFlag);
	int m_iReceCount;
};

#endif
