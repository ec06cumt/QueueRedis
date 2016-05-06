/*
 * Created on 2016/04/12 liug
 *
 */
#ifndef _MSGTRANSCEIVER_HPP_
#define _MSGTRANSCEIVER_HPP_

#include "PacketID.hpp"
#include "ZmqBus.hpp"

class CMsgTransceiver
{
public:
    // 初始化ZMQ Bus系统
    int Init();

public:
    // 发送和接收消息
    int SendOneMsg(const char* pszMsg, int iMsgLength, EGameServerID enMsgPeer);
    int RecvOneMsg(char* pszMsg, int iMaxOutMsgLen, int& riMsgLength, EGameServerID enMsgPeer);

private:

    //World 到ReidsB的ZMQ通信通道
    ZmqBus m_oWorld2ResidSvr;
};

#endif
