/*
 * Create on 2016/04/05 liug
 * 回调函数管理类
 */

#ifndef _REDISPROTOPROCMGR_H_
#define _REDISPROTOPROCMGR_H_

#include <map>
#include "GameProto.hpp"
#include "DBRedis.hpp"
#include "GuardLock.hpp"

typedef void(*LPJOB_PROC) (DBClientRedis* pRedisClient,GameMsg* pGameMsg);


class CProtoProcMgr
{
public:
	CProtoProcMgr(){};
	virtual ~CProtoProcMgr();

	LPJOB_PROC FindProc(const unsigned int uiMsgID);
	void Register(const unsigned int uiMsgID, const LPJOB_PROC proc);

	void ExeMsg(const unsigned int uiMsgID,DBClientRedis* pRedisClient,GameMsg* pGameMsg);

private:
	std::map<unsigned int, LPJOB_PROC> m_mapProc;
	pthread_mutex_t m_Mutex;
};

#endif
