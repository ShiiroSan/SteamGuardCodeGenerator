#pragma once

#include <iostream>
#include <cstdint>

std::string generateSteamGuardCodeForTime(long lTime, int mSecret[]);

std::string generateSteamGuardCode(int mSecret[]);

