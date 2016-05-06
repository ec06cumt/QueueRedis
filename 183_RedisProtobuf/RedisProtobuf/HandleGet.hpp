/*
 * Create on 2016/04/07 gliu
 */

#ifndef _REDISHANDLEGET_H_
#define _REDISHANDLEGET_H_

#include "Handle.hpp"
#include "DBRedis.hpp"
#include "PacketID.hpp"
#include <string.h>
#include <string>

//class CHandleRedisSet;

class CHandleGet :public CHandle
{
public:
	CHandleGet(DBClientRedis* pDBClientRedis);
	virtual ~CHandleGet(){};

	inline void SetThreadIdx(const int iThreadIdx){m_iThreadIdx = iThreadIdx;}
	virtual int OnExcute(GameMsg* pGameMsg);

	//virtual void OnExcuteEx(int uiMsgID,char* pBuffer,int* iLen);
private:
    // 本类的对象只能在CHandleRedisSet类中创建
//	friend class CHandleRedisSet;


	DBClientRedis* m_pDBClientRedis;
	int m_iThreadIdx;
private:
	int GetRedisValue(GameMsg* pGameMsg);
};

#endif
