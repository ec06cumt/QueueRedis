#include "RedisPacket.hpp"

CPacket::CPacket()
{
}

CPacket::~CPacket()
{
}

void CPacket::InitPacketDB()
{
	m_iLen = 0;
	m_bHasData = false;
}

void CPacket::Init(const unsigned int iPacketID)
{
	InitPacketDB();
	m_pBuffer = m_pData;

	m_iLen = 2 + 4;
	memcpy(m_pBuffer, &m_iLen, sizeof(unsigned short));
	m_pBuffer += sizeof(unsigned short);

	memcpy(m_pBuffer, &iPacketID, sizeof(unsigned int));
	m_pBuffer += sizeof(unsigned int);

	m_bHasData = true;
}

void CPacket::PushBack(const void* pData, const unsigned short iLen)
{
	if (MAX_PACKETDB_SIZE < m_iLen + iLen)
	{
		//assert(false && "MAX_PACKETDB_SIZE < m_iLen + iLen");
		return;
	}

	memcpy(m_pBuffer, pData, iLen);	m_pBuffer += iLen;
	m_iLen += iLen;
	memcpy(m_pData, &m_iLen, sizeof(unsigned short));
}

void CPacket::PushBack(const char* pData, const unsigned short iLen)
{
	if (MAX_PACKETDB_SIZE < m_iLen + iLen)
	{
		//assert(false && "MAX_PACKETDB_SIZE < m_iLen + iLen");
		return;
	}

	memcpy(m_pBuffer, pData, iLen);	m_pBuffer += iLen;
	m_iLen += iLen;
	memcpy(m_pData, &m_iLen, sizeof(unsigned short));
}

void CPacket::PushBack(const char value)
{
	PushBack(&value, sizeof(value));
}

void CPacket::PushBack(const short value)
{
	PushBack((const char*)&value, sizeof(value));
}

void CPacket::PushBack(const int value)
{
	PushBack((const char*)&value, sizeof(value));
}

//void CPacket::PushBack(const long value)
//{
//	PushBack((const char*)(const char*)&value, sizeof(value));
//}

void CPacket::PushBack(const float value)
{
	PushBack((const char*)(const char*)&value, sizeof(value));
}

void CPacket::PushBack(const double value)
{
	PushBack((const char*)&value, sizeof(value));
}

void CPacket::PushBack(const unsigned char value)
{
	PushBack((const char*)&value, sizeof(value));
}

void CPacket::PushBack(const unsigned short value)
{
	PushBack((const char*)&value, sizeof(value));
}

void CPacket::PushBack(const unsigned int value)
{
	PushBack((const char*)&value, sizeof(value));
}

//void CPacket::PushBack(const unsigned long value)
//{
//	PushBack((const char*)&value, sizeof(value));
//}

//void CPacket::PushBack(const SYSTEMTIME timeDate)
//{
//	PushBack((const char*)&timeDate, sizeof(timeDate));
//}

void CPacket::PushBack(const int64_t value)
{
	PushBack((const char*)&value, sizeof(value));
}

void CPacket::PushBack(const uint64_t value)
{
	PushBack((const char*)&value, sizeof(value));
}
