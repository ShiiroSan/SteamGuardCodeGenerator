#pragma once

#include <string>
#include <windows.h>

std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);
bool getBytes(std::string sStringToConvert, BYTE decArray[]);
