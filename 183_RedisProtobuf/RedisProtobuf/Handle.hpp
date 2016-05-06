/*
 * Create on 2016/04/05 liug
 * 回调函数基类
 */

#ifndef _REDISHANDLE_H_
#define _REDISHANDLE_H_

#include <string.h>

#include "GameProto.hpp"

class CHandle
{
protected:
	CHandle(){};
public:
	virtual ~CHandle(){};

	// 消息处理
	//virtual void OnExcuteEx(int uiMsgID,char* pBuffer,int* iLen) = 0;
	virtual int OnExcute(GameMsg* pGameMsg) = 0;
};

#endif
