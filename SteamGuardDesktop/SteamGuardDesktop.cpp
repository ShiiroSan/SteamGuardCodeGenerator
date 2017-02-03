// SteamGuardDesktop.cpp : fichier projet principal.

#include "stdafx.h"
#include <iostream>

#include "Base64.h"
#include "TimeCorrector.h"
#include "SGCPG.h"

#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

//todo: Include IMGUI and get a first look on it. 
/**
This will be directly added on this project on "Instable" branch until GUI allow to show few debugs info.
**/
using namespace System;


#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG
//todo (#me): Change mSecret to string or array of char.
BYTE mSecret[20];

int main(array<System::String ^> ^args)
{
	systemTimeSeconds();
	const std::string s = "02Id5PF74To8Ua6REX5VPC1vZcg=";
	std::string decoded = base64_decode(s);
	bool bErrorGetBytes = getBytes(decoded, mSecret);
	if (!bErrorGetBytes)
	{
		std::cout << "[ERROR] Given shared_key cannot be decrypted! Make sure it was the right one or send" <<
			" your key to eaudrey96@gmail.com" << std::endl;
		return EXIT_FAILURE;
	}
	/*DEBUGGING PURPOSE ONLY*/
	long lTime = 1473872869;

	generateSteamGuardCodeForTime(lTime, mSecret);
	//generateSteamGuardCode(mSecret);
	std::cin.get();
	return 0;
}