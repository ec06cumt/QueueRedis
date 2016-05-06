/*
 * Created on 2016/04/12 liug
 * Word 发消息
 */

#ifndef _WORLDAPP_H_
#define _WORLDAPP_H_

#include "PacketID.hpp"
#include "GameProto.hpp"
#include "ProtoEngine.hpp"
#include "MsgTransceiver.hpp"
#include <map>
class CWorldApp
{
public:
	CWorldApp(){};
	~CWorldApp(){};

	int Init();

	void Run();

	int SendMsg(int& iSendCount);
	int ReceiveMsg(int& iReceiveCount);
private:
	CMsgTransceiver m_msgTransfer;
	GameMsg m_stGameMsg;

	unsigned char m_szCode[MAX_MSGBUFFER_SIZE];

	CProtoEngine m_ProtoEngine;
	unsigned int m_uiMsgIdx;
	std::map<unsigned int,int> m_mpReceMsgID;
};

#endif
