// ConsoleApplication1.cpp : définit le point d'entrée pour l'application console.
//
//todo: Include IMGUI and get a first look on it. 
/**
This will be directly added on this project on "Instable" branch until GUI allow to show few debugs info.
**/
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
//todo (#me): Change mSecret to string or array of char.
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

