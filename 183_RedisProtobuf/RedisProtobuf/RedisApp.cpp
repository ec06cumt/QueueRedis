#include "RedisApp.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <time.h>

int CRedisApp::ms_iAppCmd = APPCMD_NOTHING_TODO;

CRedisApp::CRedisApp()
{
	memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));
	memset(m_arrQuitFlag,0,sizeof(m_arrQuitFlag));
	m_iReceCount = 0;
}

CRedisApp::~CRedisApp()
{
	printf("dected CRedisApp\n");
}

void CRedisApp::SetAppCmd(int iAppCmd)
{
	ms_iAppCmd = iAppCmd;
}

int CRedisApp::Init()
{
	int iRet = 0;
	m_ThreadMgr.SetQuitFlag(m_arrQuitFlag);

	iRet = m_ThreadMgr.Init();

	if(iRet)
	{
		printf("m_ThreadMgr init failed\n");
		return iRet;
	}
	iRet = m_stMsgTran.Init();
	if(iRet)
	{
		printf("MQ init failed\n");
		return iRet;
	}
	return 0;
}

void CRedisApp::Run()
{
//	bool bLoop = true;
	int iRecvCount = 0;
	int iSendCount = 0;
	int iLoopCount= 0;
	float time_use=0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	while(true)
	{
		//接受socket传过来的数据包
		//MQ
		while(true)
		{
			RecvMsg(iRecvCount);

			//把接受到的包，push进环形缓冲区中，让redis处理消息
			//sleep(10);

			iLoopCount++;

			PickThreadMsgAndSend(iSendCount);

			/*
			//printf("CRedisApp Run:%d\n",iRecvCount);

			if(bLoop)
			{

				std::string strKey = "";
				int idx = iRecvCount % MAX_THREAD_NUMBER;
				//if(iRecvCount == 3 || iRecvCount== 4 || iRecvCount== 5)
				{
					switch(idx)
					{
						case 3:
						{
							strKey = "key";
							break;
						}
						case 0:
						{
							strKey = "liugao";
							break;
						}
						case 1:
						{
							strKey = "test2";
							break;
						}
						case 2:
						{
							strKey = "age";
							break;
						}
					}

					CThreadRedis* pThreadRedis = m_ThreadMgr.GetThreadRedis(idx);
					if(pThreadRedis)
					{
						//printf("%d\n",pThreadRedis->GetThreadIdx());
						CJobMsg* pJobMsg = pThreadRedis->AllocDBJobMsg();
						if(pJobMsg)
						{
							//printf("Get pJobMsg success\n");

							int iSize = strKey.size();
							pJobMsg->InitMsg(GET_REDIS_KEY_EX);
							pJobMsg->PushBack(iSize);
							pJobMsg->PushBack(strKey.c_str(),strKey.size());
		//
							pThreadRedis->AddDBJobMsg(pJobMsg);
						}
					}
				}

				for(int i = 0; i< MAX_THREAD_NUMBER;i++)
				{
					CJobMsg stJobMsg;
					int iLen = 0;
					m_ThreadMgr.PopJobMsg(i,&stJobMsg,&iLen);
					if(iLen > 0)
					{
						unsigned short iLen = 0;
						int iMsgID = 0;
						int ioffset = 0;

						char* pBuffer = (char*)stJobMsg.GetData();

						memcpy(&iLen, pBuffer, sizeof(iLen));			ioffset = sizeof(iLen);
						memcpy(&iMsgID, pBuffer + ioffset, sizeof(iMsgID)); ioffset += sizeof(iMsgID);
						//printf("MsgID=%u,iLen=%d\n",iMsgID,iLen);

						int iMsgLen = 0;
						char szResult[100] = {0};
						memcpy(&iMsgLen,(pBuffer + ioffset),sizeof(iMsgLen));
						pBuffer += ioffset + (int)sizeof(iMsgLen);
						memcpy(szResult,pBuffer,iMsgLen);
						printf("iRecvCount:%d,idx=%d,MsgID:%d,szResult:%s,len:%d\n",iRecvCount
								,i,iMsgID,szResult,iMsgLen);
					}
				}

				if(iRecvCount >= 100000)
				{
					gettimeofday(&end,NULL);
					//break;
					time_use = ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))/1000000;
					printf("cost time_use:%f\n",time_use);
					bLoop = false;

					for(int i=0;i<4;i++)
					{
						m_ThreadMgr.StopThread(i);
					}
					sleep(2);
					break;
				}


			}
			//从输出缓冲区中获取数据，获取结果，发送给客户端或着，发送给返回的服务器
			//MQ
			*/
			if(iRecvCount >= 99999)
			{
				gettimeofday(&end,NULL);
				time_use = ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))/1000000;
				printf("cost time_use:%f\n",time_use);
//				usleep(2);
				printf("iLoopCount:%d,iRecvCount:%d,iRecvCount2=%d\n",iLoopCount,iRecvCount,m_iReceCount);
				break;
			}
			if(iRecvCount %100 ==0)
			{
				//sleep(1);
				break;
			//printf("=======================================iLoopCount:%d,iRecvCount:%d\n",iLoopCount,iRecvCount);
			}
		}
		usleep(10);
		if(iRecvCount > 99999)
		{
			break;
		}
		for(int i=0;i< MAX_THREAD_NUMBER;i++)
		{
			if(m_arrQuitFlag[i] >10)
			{
				printf("idx=%d,failedCount=%d\n",i,m_arrQuitFlag[i]);
				break;
			}
		}
	}


}

void CRedisApp::RecvMsg(int& iRecvCount)
{
	int iRet = ReceiveAndDispatchMsg();
	if(!iRet)
	{
		iRecvCount++;
		//printf("==============iRecCount:%d\n",iRecvCount);
	}
	else if(iRet == -5)
	{
		printf("*****************************************************Push Failed\n");
	}
}

int CRedisApp::ReceiveAndDispatchMsg()
{
	int iBufLen = sizeof(m_szCodeBuf);
	int iCodeLen;
	unsigned int userID = 0;
	memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));

	// 初始化code buffer和code length
	iCodeLen = 0;

	// 接受网络数据
	int iRet = m_stMsgTran.RecvOneMsg(m_szCodeBuf, iBufLen, iCodeLen, GAME_SERVER_WORLD);
	if ((iRet < 0) || (0 == iCodeLen))
	{
		return -1;
	}
	else
	{
		printf("receive msg success len=%d\n",iCodeLen);
		m_iReceCount++;
	}

	//char* pszRealBuff = m_szCodeBuf + sizeof(unsigned short);
	//获取UserID
	char* pszUserID =  m_szCodeBuf + sizeof(unsigned short);

	memcpy(&userID,pszUserID,(int)sizeof(unsigned int));

	//printf("size:%d,userid:%u\n",iCodeLen,userID);
	iRet = m_ThreadMgr.PushMsg(userID, m_szCodeBuf, iCodeLen);
	if (iRet != 0)
	{
		return -5;
	}

	return 0;
}

int CRedisApp::PickThreadMsgAndSend(int& riSendMsgCount)
{
	int iRealLen = 0;
	int iRet = 0;

	for(int i = 0;i< MAX_THREAD_NUMBER;i++)
	{
		memset(m_szCodeBuf,0,sizeof(m_szCodeBuf));
		iRet = m_ThreadMgr.PopJobMsg(i,m_szCodeBuf,iRealLen);
		if(iRet || iRealLen < (int)sizeof(unsigned short))
		{
			//printf("PopJobMsg failded\n");

		}
		else
		{
			printf("PopJobMsg:iRealLen=%d\n",iRealLen);
			// 发送网络数据
			int iRet = m_stMsgTran.SendOneMsg(m_szCodeBuf, iRealLen, GAME_SERVER_WORLD);
			if (iRet < 0)
			{
				printf("SendOneMsg failded\n");
				return -1;
			}
			else
			{
//				printf("PopJobMsg success %d\n",iRealLen);
				riSendMsgCount++;
			}
		}
	}

	return 0;

}

void CRedisApp::GetQuitFlag(int& iQuitFlag)
{
	for(int i = 0;i< MAX_THREAD_NUMBER;i++)
	{

	}
}
