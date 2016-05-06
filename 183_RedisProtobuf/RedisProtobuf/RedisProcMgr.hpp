/*
 * Create on 2016/04/05 liug
 * 回调函数管理类
 */

#ifndef _REDISPROCMGR_H_
#define _REDISPROCMGR_H_

#include <map>
typedef void(*LPDBJOB_PROC) (const char* pBuffer, const unsigned int& dwlen);


class CProcMgr
{
public:
	CProcMgr(){};
	virtual ~CProcMgr();

	LPDBJOB_PROC FindProc(const unsigned int packetID);
	void Register(const unsigned int packetID, const LPDBJOB_PROC proc);

private:
	std::map<unsigned int, LPDBJOB_PROC> m_mapProc;
};

#endif
