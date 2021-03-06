﻿/*
 *	Create on 2016/04/02 gliu
 *	协议ID
 */

#ifndef _REDISPACKETID_H_
#define _REDISPACKETID_H_

#define MAX_HANDLER_NUMBER 16384
#define MAX_REDIS_KEY_LEN 128
const unsigned int MAX_MSGBUFFER_SIZE =  20480;
enum
{
	GET_REDIS_KEY_EX = 101,
	MAX_REDIS_KEY
};

enum
{
	GET_REDIS_REPLY_EX = 102,
	MAX_REDIS_REPLY_KEY
};

// Server服务器ID
typedef enum enGameServerID
{
    GAME_SERVER_UNDEFINE        = -1,
    GAME_SERVER_WORLD           = 0,
	GAME_SERVER_REDIS           = 1,

} EGameServerID;

const unsigned int GET_REDIS_KEY = 100;

const int APPCMD_NOTHING_TODO   = 0;          //啥也不干
const int APPCMD_STOP_SERVICE   = 1;          //停止命令
const int APPCMD_RELOAD_CONFIG  = 2;          //重读配置命令
const int APPCMD_QUIT_SERVICE   = 3;          //立刻跳出循环

#endif
