#include "MsgTransceiver.hpp"

int CRedisMsgTransceiver::Init()
{

	int iRet = m_oWorld2ResidSvr.ZmqInit("127.0.0.1:7788", EN_ZMQ_SOCKET_PAIR, EN_ZMQ_PROC_TCP, EN_ZMQ_SERVICE_SERVER);
	if(iRet)
	{
		return iRet;
	}

	return iRet;
}

int CRedisMsgTransceiver::SendOneMsg(const char* pszMsg, int iMsgLength, EGameServerID enMsgPeer)
{
	int iRet = 0;

	switch (enMsgPeer)
	{
	case GAME_SERVER_WORLD:
		{
			iRet = m_oWorld2ResidSvr.ZmqSend(pszMsg, iMsgLength, 0);


			break;
		}

	default:
		{
			break;
		}
	}

	return iRet;
}

int CRedisMsgTransceiver::RecvOneMsg(char* pszMsg, int iMaxOutMsgLen, int& riMsgLength, EGameServerID enMsgPeer)
{
	int iRet = 0;

	switch (enMsgPeer)
	{
	case GAME_SERVER_WORLD:
		{
			iRet = m_oWorld2ResidSvr.ZmqRecv(pszMsg, iMaxOutMsgLen, riMsgLength, 0);

			break;
		}

	default:
		{
			break;
		}
	}

	return iRet;
}


