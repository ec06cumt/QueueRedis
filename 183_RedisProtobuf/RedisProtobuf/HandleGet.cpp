#include "HandleGet.hpp"

CHandleGet::CHandleGet(DBClientRedis* pDBClientRedis)
{
	printf("CHandleGet::CHandleGet\n");
	m_pDBClientRedis = pDBClientRedis;

}

int CHandleGet::OnExcute(GameMsg* pGameMsg)
{
	int iRet = 0;
	if(!pGameMsg)
	{
		return -1;
	}
	switch(pGameMsg->m_stmsghead().m_uimsgid())
	{
		case PROTO_MSGID_GETINFO_REQUEST:
		{
			iRet = GetRedisValue(pGameMsg);
			break;
		}
		default:
		{
			iRet = -1;
			break;
		}
	}
	return iRet;
}
int CHandleGet::GetRedisValue(GameMsg* pGameMsg)
{
	if(!m_pDBClientRedis || !pGameMsg)
	{
		return -1;
	}

	//每次操作都要进行连接的判断
	std::string strHost = "127.0.0.1";
	int iPort = 6379;
	bool bConnected = m_pDBClientRedis->ConnectRedis(strHost,iPort);
	if(!bConnected)
	{
		return -2;
	}
	std::string strkey = pGameMsg->m_stmsgbody().m_stuserkeyrequst().strkey();
	if((int)strkey.size() == 0)
	{
		return -3;
	}
	std::string strValue;
	m_pDBClientRedis->Get(strkey,strValue);
	printf("Key:%s,Get Value:%s\n",strkey.c_str(),strValue.c_str());

	Redis_GetUserInfo_Response* pResponse = pGameMsg->mutable_m_stmsgbody()->mutable_m_stuserkeyresponse();
	if(!pResponse)
	{
		printf("New Response failed\n");
		return -4;
	}
	pResponse->set_strvalue(strValue);

	//printf("szKey=%s,strValue=%s\n",strKey.c_str(),value.c_str());
	return 0;
}

//void CHandleGet::OnExcuteEx(int uiMsgID,char* pBuffer,int* iLen)
//{
//	return;
//}
