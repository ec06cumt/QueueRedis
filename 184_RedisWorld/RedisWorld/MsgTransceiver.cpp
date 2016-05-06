#include "MsgTransceiver.hpp"

int CMsgTransceiver::Init()
{

	int iRet = m_oWorld2ResidSvr.ZmqInit("127.0.0.1:7788", EN_ZMQ_SOCKET_PAIR, EN_ZMQ_PROC_TCP, EN_ZMQ_SERVICE_CLIENT);
	if(iRet)
	{
		printf("CMsgTransceiver::Init failed\n");
	}

	return iRet;
}

int CMsgTransceiver::SendOneMsg(const char* pszMsg, int iMsgLength, EGameServerID enMsgPeer)
{
	int iRet = 0;

	switch (enMsgPeer)
	{
	case GAME_SERVER_REDIS:
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

int CMsgTransceiver::RecvOneMsg(char* pszMsg, int iMaxOutMsgLen, int& riMsgLength, EGameServerID enMsgPeer)
{
	int iRet = 0;

	switch (enMsgPeer)
	{
	case GAME_SERVER_REDIS:
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


