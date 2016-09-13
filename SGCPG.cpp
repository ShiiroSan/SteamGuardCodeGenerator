#include "stdafx.h"
#include "TimeCorrector.h"
#include "SGCPG.h"
#include <Windows.h>
#include <windows.system.h>
#include <wincrypt.h>      // CryptoAPI definitions

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG

typedef cryptohash_t<CALG_SHA1> sha1_t;

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
		//TODO (#me1.1): Make SHA1 func
		//TODO: (#me1.2) Make HMAC func following AutoIT ones.
		std::string text = "mariusbancila";
		std::string digest;

		sha1_t hasher;
		if (hasher.begin())
		{
			if (hasher.update((unsigned char*)(text.c_str()), text.length()))
			{
				if (hasher.finalize())
				{
					digest = hasher.hexdigest();
				}
			}
		}

		if (digest.empty())
			std::cout << "error code = " << hasher.lasterror().errorCode
			<< ", error message = " << hasher.lasterror().errorMessage
			<< std::endl;
		else
			DEBUG_MSG(digest);
		//TODO (#me2): Give a look to bytebuffer (java)
	}
	return steamCode;
}

std::string generateSteamGuardCode(int mSecret[])
{
	return generateSteamGuardCodeForTime(currentTime(), mSecret);
}
