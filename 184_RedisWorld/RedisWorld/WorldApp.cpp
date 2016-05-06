#include "WorldApp.hpp"


int CWorldApp::Init()
{
	memset(m_szCode,0,sizeof(m_szCode));
	int iRet = m_msgTransfer.Init();
	m_ProtoEngine.Initialize(0);
	m_uiMsgIdx = 0;
	return iRet;
}

void CWorldApp::Run()
{
	int iSendCount = 0;
	int iRecvCount = 0;
	int iRet = 0;
//	unsigned int uiUserID = 1001;
//	unsigned int uiSessionFD = 10001;
	while(true)
	{
		if(iSendCount % 100== 0)
		{
//			std::map<unsigned int,int>::iterator it = m_mpReceMsgID.begin();
//			for(;it != m_mpReceMsgID.end();it++)
//			{
//				printf("failed MsgID=%u\n",it->first);
//			}
			usleep(10);

		}
		ReceiveMsg(iRecvCount);

//		iSendCount++;

		iRet = SendMsg(iSendCount);
		if(!iRet)
		{
			printf("Send Msg success idx=%d\n",iSendCount);
		}
		else
		{
			printf("Send Msg failed idx=%d\n",iSendCount);
		}

		if(iRecvCount >= 10000)
		{
			printf("iRecvCount %d\n",iRecvCount);
			break;
		}



	}
}

int CWorldApp::ReceiveMsg(int& iReceiveCount)
{
	int iBufLen = sizeof(m_szCode);
	int iCodeLen = 0;
	unsigned int userID = 0;
	memset(m_szCode,0,sizeof(m_szCode));

	// 初始化code buffer和code length

	// 接受网络数据
	int iRet = m_msgTransfer.RecvOneMsg((char*)m_szCode, iBufLen, iCodeLen, GAME_SERVER_REDIS);
	if ((iRet < 0) || (iCodeLen < (int)sizeof(unsigned short)))
	{
		return -1;
	}
	else
	{
		//printf("+++++++++++++++Recve code success iLen=%d\n",iCodeLen);
		iReceiveCount++;
	}

	//char* pszRealBuff = m_szCodeBuf + sizeof(unsigned short);
	//获取UserID
	char* pszUserID =  (char*)m_szCode + sizeof(unsigned short);

	memcpy(&userID,pszUserID,(int)sizeof(unsigned int));

	m_stGameMsg.Clear();
	iRet =m_ProtoEngine.Decode(m_szCode,iCodeLen,&m_stGameMsg);
	if(!iRet)
	{
		Redis_GetUserInfo_Request stRequest = m_stGameMsg.m_stmsgbody().m_stuserkeyrequst();
		Redis_GetUserInfo_Response stResponse = m_stGameMsg.m_stmsgbody().m_stuserkeyresponse();
		unsigned int uiMsgID = m_stGameMsg.m_stmsghead().m_uimsgid();
		printf("Receive code OK, Msg UserID=%u,Uin = %u, MsgID = %u,strKey=%s,strValue=%s,MsgIdx=%u\n"
				,userID
				,m_stGameMsg.m_stmsghead().m_uid()
				,uiMsgID
				,stRequest.strkey().c_str()
				,stResponse.strvalue().c_str()
				,m_stGameMsg.m_stmsghead().m_msgidx());

		std::map<unsigned int,int>::iterator it = m_mpReceMsgID.find(m_stGameMsg.m_stmsghead().m_msgidx());
		if(it != m_mpReceMsgID.end())
		{
			m_mpReceMsgID.erase(it);
		}
	}

	return 0;
}

int CWorldApp::SendMsg(int& iSendCount)
{
	unsigned int uiUserID = 0;
	unsigned int uiSessionFD = 10001;
	memset(m_szCode,0,sizeof(m_szCode));
	m_stGameMsg.Clear();

	GameMsgHead* pMsgHead = m_stGameMsg.mutable_m_stmsghead();

	pMsgHead->Clear();

	pMsgHead->set_m_uimsgid((ProtoMsgID)PROTO_MSGID_GETINFO_REQUEST);

	pMsgHead->set_m_uisessionfd(uiSessionFD);
	pMsgHead->set_m_strsessionkey("ABCDEFG");

	Redis_GetUserInfo_Request* pMsgRequest = m_stGameMsg.mutable_m_stmsgbody()->mutable_m_stuserkeyrequst();

	int idx = iSendCount % 4;
	switch(idx)
	{
	case 0:
	{
		uiUserID = 1001;
		pMsgRequest->set_strkey("liugao");
		break;
	}
	case 1:
	{
		uiUserID = 1002;
		pMsgRequest->set_strkey("test2");
		break;
	}
	case 2:
	{
		uiUserID = 1003;
		pMsgRequest->set_strkey("age");
		break;
	}
	case 3:
	{
		uiUserID = 1004;
		pMsgRequest->set_strkey("key");
		break;
	}
	default:
	{
		pMsgRequest->set_strkey("key");
					break;
	}
	}

	//m_uiMsgIdx++;
	pMsgHead->set_m_msgidx(iSendCount);
	pMsgHead->set_m_uid(uiUserID);

	int iProtoSize= m_stGameMsg.ByteSize();
	iProtoSize = (int)sizeof(m_szCode);
	int iLenSize = 0;

	int iRet = m_ProtoEngine.Encode(&m_stGameMsg,m_szCode,iProtoSize,iLenSize);
	if(iRet)
	{
		printf("Encode Failed,%d\n",iLenSize);
		return -1;
	}
	else
	{

		printf("Encode sucess,%d\n",iLenSize);

	}
	m_mpReceMsgID.insert(std::make_pair(iSendCount,1));

	iRet = m_msgTransfer.SendOneMsg((const char*)m_szCode,iLenSize,GAME_SERVER_REDIS);
	if(iRet < 0)
	{
		printf("SendOneMsg failed\n");
	}
	else
	{
		iSendCount++;
	}

	return 0;
}
