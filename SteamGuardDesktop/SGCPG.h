#pragma once

#include <iostream>
#include <cstdint>
#include <windows.h>

std::string generateSteamGuardCodeForTime(long lTime, BYTE mSecret[]);

std::string generateSteamGuardCode(BYTE mSecret[]);

void hmacsha1(BYTE password[], BYTE message[]);