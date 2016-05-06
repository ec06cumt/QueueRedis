#include <stdio.h>
#include <stdlib.h>
#include "Singleton.h"
#include "WorldApp.hpp"

int main(int argc, char **argv)
{
	int iRet = 0;
	iRet = Singleton<CWorldApp>::GetInstance().Init();
	if(iRet)
	{
		printf("init failed\n");
		return -1;
	}

	Singleton<CWorldApp>::GetInstance().Run();
	//CRedisApp* pRedisApp = new CRedisApp();
	//redisApp.Init();

	return 0;
}
