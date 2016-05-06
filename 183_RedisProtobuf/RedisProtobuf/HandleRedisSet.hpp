/*
 * Create on 2016/04/07 gliu
 * HandleMgr.hpp
 * 处理Redis的函数集合
 *
 */

#ifndef _REDISHANDLEREDISSET_H_
#define _REDISHANDLEREDISSET_H_

#include "HandleSet.hpp"
#include "DBRedis.hpp"
#include "HandleGet.hpp"

class CHandleRedisSet:public CHandleSet
{
public:
	CHandleRedisSet();
	virtual ~CHandleRedisSet(){};

	virtual int Init(int idx);

private:
	int RegisterAllHandler();
	CHandleGet m_HandleGet;

private:
	int m_iThreadIdx;
	 DBClientRedis m_dbClientRedis;

};

#endif
