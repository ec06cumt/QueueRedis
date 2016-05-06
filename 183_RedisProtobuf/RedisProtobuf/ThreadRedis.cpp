#include "ThreadRedis.hpp"
#include "Singleton.h"
#include "Handle.hpp"

//DBClientRedis CThreadRedis::m_RedisClient;
//GameMsg CThreadRedis::m_stGameMsg;

CThreadRedis::CThreadRedis():m_bEndThread(0)
{
	m_pCodeQueue = NULL;
	m_pMyCodeQueue = NULL;
	m_pMyCodeOutQueue = NULL;
	memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));
	memset(m_szMyCodeBuf,0,sizeof(m_szCodeBuf));
	m_stGameMsg.Clear();
	m_uiMaxIdx = 0;
	m_pFailedCount = NULL;
}

CThreadRedis::~CThreadRedis()
{
	m_bEndThread = 1;
	sleep(3);

	printf("detected CThreadRedis %d\n",m_iThreadIdx);
	if(m_pCodeQueue)
	{
		delete m_pCodeQueue;
		m_pCodeQueue = NULL;
		printf("delete m_pCodeQueue\n");
	}
	if(m_pMyCodeQueue)
	{
		delete m_pMyCodeQueue;
		m_pMyCodeQueue = NULL;
		printf("delete m_pMyCodeQueue\n");
	}

	if(m_pMyCodeOutQueue)
	{
		delete m_pMyCodeOutQueue;
		m_pMyCodeOutQueue = NULL;
		printf("delete m_pMyCodeOutQueue\n");
	}
}

void* ThreadProc( void *pvArgs )
{
	if( !pvArgs )
	{
		return NULL;
	}

	CThreadRedis *pThread = (CThreadRedis *)pvArgs;

	//run the thread logic
	if(pThread)
	{
		int iCount = 0;
		while(true)
		{
			iCount++;
//			pThread->Process();
			pThread->MyProcess();
			//判断线程是否结束
			if(pThread->GetEndFlag())
			{
				break;
			}
			usleep(1);
		}

		pThread->ProcessAll();

	}
	printf("Thread Stop %d\n",pThread->GetThreadIdx());

	return NULL;
}


int CThreadRedis::Init(int iThreadIndex)
{
	printf("Thread begin success %d\n",iThreadIndex);
	int iRet = 0;
	m_iThreadIdx = iThreadIndex;

	const int CODEQUEUESIZE = 10240;

	m_pCodeQueue = new CCodeQueue(CODEQUEUESIZE);
	if(!m_pCodeQueue)
	{
		return -1;
	}
	m_pCodeQueue->Initialize(false);

	m_pMyCodeQueue = new CCodeQueueA(CODEQUEUESIZE);
	if(!m_pMyCodeQueue)
	{
		printf("New m_pMyCodeQueue failed\n");
		return -1;
	}

	m_pMyCodeOutQueue = new CCodeQueueA(CODEQUEUESIZE);
	if(!m_pMyCodeOutQueue)
	{
		printf("New m_pMyCodeOutQueue failed\n");
		return -1;
	}
	m_uiMaxIdx = 0;
	m_ProtoEngine.Initialize(iThreadIndex);

//	REGISTER_JOB_PROC(PROTO_MSGID_GETINFO_REQUEST);
//	Singleton<CProtoProcMgr>::GetInstance().Register(PROTO_MSGID_GETINFO_REQUEST,FUNC_JOB(PROTO_MSGID_GETINFO_REQUEST));
//	m_ProtoProcMgr.Register(PROTO_MSGID_GETINFO_REQUEST,FUNC_JOB(PROTO_MSGID_GETINFO_REQUEST));

	m_HandRedisSet.Init(iThreadIndex);

	//create thread
	iRet = CreateThread();
	if (iRet != 0)
	{
		printf("iRet:%d\n",iRet);
		return -12;
	}

	printf("Thread Init success %d\n",iThreadIndex);
	return iRet;
}

int CThreadRedis::CreateThread()
{
	int iRt = 0;
	iRt = pthread_attr_init(&m_stAttr);
	if (iRt != 0)
	{
		printf("pthread_attr_init\n");
		return -1;
	}

	// to complete with system thread
	iRt = pthread_attr_setscope(&m_stAttr, PTHREAD_SCOPE_SYSTEM);
	if (iRt != 0)
	{
		printf("pthread_attr_setscope\n");
		return -3;
	}

	iRt = pthread_attr_setdetachstate(&m_stAttr, PTHREAD_CREATE_JOINABLE);
	if (iRt != 0)
	{
		printf("pthread_attr_setdetachstate\n");
		return -5;
	}

	iRt = pthread_create(&m_hThread, &m_stAttr, ThreadProc, (void *)this);
	if (iRt != 0)
	{
		printf("pthread_create\n");
		return -7;
	}

	return 0;
}


void CThreadRedis::MyProcess()
{
	int iRet = 0;
	int iLen = 0;
	while (true)
	{
		iLen = 0;
		iRet = 0;
		//每次循环之前都清空之前的记录
		m_stGameMsg.Clear();
		memset(m_szMyCodeBuf,0,sizeof(m_szCodeBuf));

		iRet = m_pMyCodeQueue->GetHeadCode(m_szMyCodeBuf, &iLen);
		if(iRet < 0 || iLen <= (int)sizeof(unsigned short))
		{
			//printf("%d_MyProcess break\n",m_iThreadIdx);
			break;
		}
		else
		{
			//printf("total size:%d\n",iLen);
			iRet = m_ProtoEngine.Decode((unsigned char*)m_szMyCodeBuf,iLen,&m_stGameMsg);
			if(!iRet)
			{
//				Redis_GetUserInfo_Request stRequest = m_stGameMsg.m_stmsgbody().m_stuserkeyrequst();
//				unsigned int uiMsgID = m_stGameMsg.m_stmsghead().m_uimsgid();
//				printf("Receive code OK, idx = %d,Uin = %u, MsgID = %u,strKey=%s,MsgIdx=%u\n"
//						,m_iThreadIdx
//						,m_stGameMsg.m_stmsghead().m_uid()
//						,uiMsgID
//						,stRequest.strkey().c_str()
//						,m_stGameMsg.m_stmsghead().m_msgidx());

//				LPJOB_PROC pJobProc = Singleton<CProtoProcMgr>::GetInstance().FindProc(uiMsgID);
//				Singleton<CProtoProcMgr>::GetInstance().ExeMsg(uiMsgID,&m_RedisClient,&m_stGameMsg);
//				pJobProc(&m_RedisClient,&m_stGameMsg);

				iRet = ProcessMsg();
				if(iRet == 0)
				{
					EncodeAndSendMsg();
				}

			}
			else
			{
				printf("Decode Protocol buf failed %d\n",iRet);
			}
		}

	}
}

void CThreadRedis::Process()
{
	int iRet = 0;
	int iLen = 0;
	while (true)
	{
		iLen = 0;
		iRet = 0;
		memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));
		int iMaxLength = MAX_MSGBUFFER_SIZE - 1;
		iRet = m_pCodeQueue->PopOneCode(m_szCodeBuf, iMaxLength, iLen);
		if(iRet < 0 || iLen <= (int)sizeof(int))
		{
			//printf("%d_Process break\n",m_iThreadIdx);
			break;
		}

	}
}

int CThreadRedis::PushCode(char* pMsg,int iLen)
{
	if(!pMsg)
	{
		return -1;
	}
	int iRet = m_pMyCodeQueue->AppendOneCode(pMsg,iLen);
	if(iRet < 0)
	{
		(*m_pFailedCount)++;
		return iRet;
	}
	return 0;
}

void CThreadRedis::ProcessAll()
{
	int iRet = 0;
	int iLen = 0;
	while (true)
	{
		iLen = 0;
		iRet = 0;
		memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));
		int iMaxLength = MAX_MSGBUFFER_SIZE - 1;
		iRet = m_pCodeQueue->PopOneCode(m_szCodeBuf, iMaxLength, iLen);
		if(iRet < 0 || iLen <= (int)sizeof(int))
		{
			//printf("%d_Process break\n",m_iThreadIdx);
			break;
		}

	}
}

void CThreadRedis::MyProcessAll()
{
	int iRet = 0;
	int iLen = 0;
	while (true)
	{
		iLen = 0;
		iRet = 0;
		memset(m_szMyCodeBuf,0,sizeof(m_szCodeBuf));
		iRet = m_pMyCodeQueue->GetHeadCode(m_szMyCodeBuf, &iLen);
		if(iRet < 0 || iLen <= (int)sizeof(int))
		{
			//printf("%d_MyProcess break\n",m_iThreadIdx);
			break;
		}
	}
}

int CThreadRedis::ProcessMsg()
{
	CHandle* pHandle = m_HandRedisSet.GetHandle(m_stGameMsg.m_stmsghead().m_uimsgid());
	if(pHandle)
	{
		return pHandle->OnExcute(&m_stGameMsg);
	}
	else
	{
		printf("Failed to Get Handle\n");
	}
	return -4;
}

int CThreadRedis::EncodeAndSendMsg()
{
	memset(m_szMyCodeBuf,0,sizeof(m_szMyCodeBuf));
	int iBuffLen = (int)sizeof(m_szMyCodeBuf);
	int iRealLen = 0;
	int iRet = m_ProtoEngine.Encode(&m_stGameMsg,(unsigned char*)m_szMyCodeBuf,iBuffLen,iRealLen);
	if(iRet != 0)
	{
//		printf("EncodeAndSendMsg Encode code failed\n");
		return -1;
	}

	if(iRealLen < ((int)sizeof(unsigned short) + (int)sizeof(unsigned int)))
	{
//		printf("Invalid code\n");
		return -2;
	}
	if(m_uiMaxIdx < m_stGameMsg.m_stmsghead().m_msgidx())
	{
		m_uiMaxIdx = m_stGameMsg.m_stmsghead().m_msgidx();
	}
	iRet = m_pMyCodeOutQueue->AppendOneCode(m_szMyCodeBuf, iRealLen);
	if(iRet < 0)
	{
//		printf("EncodeAndSendMsg m_pMyCodeOutQueue Add Msg failed\n");
	}
	else
	{
//		printf("AppendOneCode success iReallen =%d\n",iRealLen);
		printf("++++++++++++++++++++++++++++++++++++++SendMaxIdx=%d\n",m_uiMaxIdx);
	}
	return 0;
}

int CThreadRedis::PopCode(char* pMsg,int& iLen)
{
	iLen = 0;
	int iRet = m_pMyCodeOutQueue->GetHeadCode(pMsg, &iLen);
	if(iRet < 0 || iLen < (int)sizeof(unsigned short))
	{
		if(iLen > 0)
		{
			printf("PopCode failed iLen%d\n",iLen);
		}
		return -1;
	}
	else
	{
		printf("GetHeadCode success iLen=%d\n",iLen);
	}

	return 0;
}

void CThreadRedis::SetFailedFlag(int* pFailedCount)
{
	if(pFailedCount)
	{
		m_pFailedCount = pFailedCount;
	}
}

//DEFINE_JOB_PROC(PROTO_MSGID_GETINFO_REQUEST)
//{
//	if(!pRedisClient || !pRedisClient->IsConnected())
//	{
//		printf("Connected Redis Failed\n");
//		return;
//	}
//	if(!pGameMsg)
//	{
//		printf("pGameMsg is null\n");
//		return;
//	}
//
//	std::string strKey = m_stGameMsg.m_stmsgbody().m_stuserkeyrequst().strkey();
//	std::string strValue;
//	pRedisClient->Get(strKey,strValue);
//
//	if(strValue.size() > 0)
//	{
//		printf("strKey:%s,strValue:%s\n",strKey.c_str(),strValue.c_str());
//	}
//	else
//	{
//		printf("Get Key Value failed\n");
//	}
//}
