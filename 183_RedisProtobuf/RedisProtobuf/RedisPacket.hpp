/*
 * Create on 2016/04/05 liug
 * 基础包类
 *
 */

#ifndef _REDISPACKET_H_
#define _REDISPACKET_H_

#include "Int64Utility.hpp"
#include <string.h>

#define MAX_PACKETDB_SIZE 10240

class CPacket
{
public:
	CPacket();
	~CPacket();

	const bool HasData() const { return m_bHasData; };
	const char* GetData() { return m_pData; };
	const unsigned short GetLength() const { return m_iLen; };
	void Init(const unsigned int iPacketID);


	void PushBack(const void* pData,const unsigned short iLen);
	void PushBack(const char* pData, const unsigned short iLen);

	void PushBack(const char value);
	void PushBack(const short value);
	void PushBack(const int value);
//	void PushBack(const long value);
	void PushBack(const float value);
	void PushBack(const double value);

	void PushBack(const unsigned char value);
	void PushBack(const unsigned short value);
	void PushBack(const unsigned int value);
//	void PushBack(const unsigned long value);
//	void PushBack(const SYSTEMTIME timeDate);
	void PushBack(const int64_t value);
	void PushBack(const uint64_t value);

protected:
	unsigned short m_iLen;
	char* m_pBuffer;
	char* m_pData;
	bool m_bHasData;

	void InitPacketDB();
};

#endif
