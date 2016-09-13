#include "stdafx.h"
#include "TimeCorrector.h"
#include "SGCPG.h"
#include <Windows.h>
#include <windows.system.h>

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG

const int SGCharsLength = 27;
unsigned char s_cgchSteamguardCodeChars[SGCharsLength] = "23456789BCDFGHJKMNPQRTVWXY";


std::string generateSteamGuardCodeForTime(long lTime, int mSecret[])
{
	std::string steamCode;
	if (mSecret == NULL)
		steamCode = "";
	else
	{
		lTime /= 30L;
		unsigned char mainComponent[8];
		int n2 = 8;
		while (true)
		{
			int n3 = n2 - 1;
			if (n2 <= 0)
			{
				break;
			}
			mainComponent[n3] = (unsigned char)lTime;
			DEBUG_MSG("mainComponent[n3] with pos: " << n3 << " worth " << (int)mainComponent[n3]);
			DEBUG_MSG("lTime before bitwise worth " << lTime);
			lTime = lTime >> 8;
			DEBUG_MSG("lTime after bitwise worth " << lTime);
			n2 = n3;
		}
	}
	return steamCode;
}

std::string generateSteamGuardCode(int mSecret[])
{
	return generateSteamGuardCodeForTime(currentTime(), mSecret);
}
