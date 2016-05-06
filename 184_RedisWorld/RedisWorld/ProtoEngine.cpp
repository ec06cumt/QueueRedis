#include "ProtoEngine.hpp"
#include <stdio.h>

CProtoEngine::CProtoEngine()
{
	m_iThreadIdx = 0;
}

int CProtoEngine::Initialize(const int iThreadIdx)
{
	m_iThreadIdx = iThreadIdx;
	return 0;
}
int CProtoEngine::Decode(unsigned char* pszCodeBuf, const int iCodeLen, GameMsg* pstMsg)
{
	if (!pszCodeBuf || !pstMsg)
	{
		return -1;
	}

	// 网络数据
	int iBuffLen = iCodeLen - sizeof(unsigned short) - sizeof(unsigned int);
	unsigned char* pszBuff = pszCodeBuf + sizeof(unsigned short) + sizeof(unsigned int);

	//解码
	bool bRet = pstMsg->ParseFromArray(pszBuff, iBuffLen);
	if(!bRet)
	{
		return -2;
	}

	return 0;
}

int CProtoEngine::Encode(GameMsg* pstMsg, unsigned char* pszCodeBuf, int iBufLen, int& iCodeLen)
{
	// 不使用NetHead
	if (!pstMsg)
	{
		return -1;
	}

	if (!pszCodeBuf)
	{
		return -3;
	}
	//对protobuf进行编码
	unsigned int uiUserID = pstMsg->m_stmsghead().m_uid();


	int iProtoMsgLen = pstMsg->ByteSize();
	if(iProtoMsgLen >= iBufLen)
	{
		return -4;
	}

	iCodeLen = iProtoMsgLen + sizeof(unsigned short)+sizeof(unsigned int);
	pszCodeBuf[0] = iCodeLen / 256;
	pszCodeBuf[1] = iCodeLen % 256;
	//添加玩家ID
	memcpy((pszCodeBuf+sizeof(unsigned short)),&uiUserID,sizeof(unsigned int));


	unsigned char* pCharArray = pszCodeBuf + sizeof(unsigned short) + sizeof(unsigned int);
	// 编码数据
	bool bRet = pstMsg->SerializeToArray(pCharArray, iBufLen);
	if(!bRet)
	{
		return -5;
	}

//	GameMsg msg;
//	int iRet = Decode(pszCodeBuf,iCodeLen,&msg);
//	if(iRet)
//	{
//		//printf("iRet:%d\n",iRet);
//	}
//	else
//	{
//		//printf("Decode success\n");
//	}

//	char szTest[1024]={0};
//	int iTestlen = sizeof(szTest);
//	bRet = pstMsg->SerializeToArray(szTest, iTestlen);
//	if(!bRet)
//	{
//		printf("SerializeToArray test failed\n");
//		return -5;
//	}
//	else
//	{
//		printf("SerializeToArray test success\n");
//	}
//	msg.Clear();
//	bRet = msg.ParseFromArray(szTest, iProtoMsgLen);
//	if(!bRet)
//	{
//		printf("ParseFromArray Test failed\n");
//		return -2;
//	}
//	else
//	{
//		printf("ParseFromArray Test success\n");
//		printf("Uid:%u\n",msg.m_stmsghead().m_uid());
//	}

	return 0;
}
