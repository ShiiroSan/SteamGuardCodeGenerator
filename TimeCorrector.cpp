#include "stdafx.h"
#include <iostream>
#include "time.h"

#include "TimeCorrector.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG

long systemTimeSeconds()
{
	time_t timeInfo;
	long lSeconds = time(&timeInfo);
	DEBUG_MSG("[debug] seconds from systemTimeSeconds: " << lSeconds);
	return lSeconds;
}

long currentTime()
{
	return systemTimeSeconds();
}

int timeBeforeNextChange(long lPassedTime/*=0*/)
{
	if (!lPassedTime) lPassedTime = systemTimeSeconds();
	DEBUG_MSG("[debug] passed time in timeBeforeNextChange: " << lPassedTime);
	int iRemainingTimeForCode = (30L - lPassedTime % 30L);
	DEBUG_MSG("[debug] time left for code in timeBeforeNextChange: " << iRemainingTimeForCode);
	return iRemainingTimeForCode;
}
