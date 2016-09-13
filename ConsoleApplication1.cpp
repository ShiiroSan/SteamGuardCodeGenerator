// ConsoleApplication1.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>

#include "Base64.h"
#include "TimeCorrector.h"
#include "SGCPG.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG

using namespace std;

int mSecret[20];

int main()
{
	systemTimeSeconds();
	const string s = "02Id5PF74To8Ua6REX5VPC1vZcg=";
	string decoded = base64_decode(s);
	bool bErrorGetBytes = getBytes(decoded, mSecret);
	if (!bErrorGetBytes)
	{
		cout << "[ERROR] Given shared_key cannot be decrypted! Make sure it was the right one or send" <<
			" your key to eaudrey96@gmail.com" << endl;
		return EXIT_FAILURE;
	}
	generateSteamGuardCode(mSecret);
	cin.get();
	return 0;
}

