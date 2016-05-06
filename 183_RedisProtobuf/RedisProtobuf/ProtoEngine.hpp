/*
 *	Create on 2016/04/02 gliu
 *	MQ协议转换通用类
 */

#ifndef _PROTOENGINE_H_
#define _PROTOENGINE_H_



#include "GameProto.hpp"

class CProtoEngine
{
public:
	CProtoEngine();
	~CProtoEngine(){};
    int Initialize(const int iThreadIdx);
    int Decode(unsigned char* pszCodeBuf, const int iCodeLen, GameMsg* pstMsg);
    int Encode(GameMsg* pstMsg, unsigned char* pszCodeBuf, int iBufLen, int& iCodeLen);

private:
	int m_iThreadIdx;
};

#endif
