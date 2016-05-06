/*
 * Create on 2016/04/07 liug
 * HandleSet.hpp
 * 处理Redis的函数集合
 *
 */

#ifndef _REDISHANDLESET_H_
#define _REDISHANDLESET_H_

#include "Handle.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <map>

#define MAX_HANDLER_NUMBER 16384

class CHandleSet
{
protected:
	CHandleSet();
public:
	virtual ~CHandleSet(){};
	virtual int Init(int idx);
	CHandle* GetHandle(const unsigned int iHandleID);

protected:
	int RegisterHandler(const unsigned int iHandleID,CHandle* pHandle);

	std::map<unsigned int,CHandle*> m_mpHandle;



	//CHandle* m_arrPHandle[MAX_HANDLER_NUMBER];



};

#endif
