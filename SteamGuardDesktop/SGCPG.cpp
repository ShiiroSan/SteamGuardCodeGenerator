#include "stdafx.h"
#include "TimeCorrector.h"
#include "SGCPG.h"
#include <wincrypt.h>      // CryptoAPI definitions

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif //DEBUG

const int SGCharsLength = 27;
unsigned char s_cgchSteamguardCodeChars[SGCharsLength] = "23456789BCDFGHJKMNPQRTVWXY";


std::string generateSteamGuardCodeForTime(long lTime, BYTE mSecret[])
{
	std::string steamCode;
	if (mSecret == NULL)
		steamCode = "";
	else
	{
		lTime /= 30L;
		BYTE mainComponent[8];
		int n2 = 8;
		while (true)
		{
			int n3 = n2 - 1;
			if (n2 <= 0)
			{
				break;
			}
			mainComponent[n3] = (BYTE)lTime;
			DEBUG_MSG("mainComponent[n3] with pos: " << n3 << " worth " << (int)mainComponent[n3]);
			DEBUG_MSG("lTime before bitwise worth " << lTime);
			lTime = lTime >> 8;
			DEBUG_MSG("lTime after bitwise worth " << lTime);
			n2 = n3;
		}
		hmacsha1(mSecret, mainComponent);
		//TODO (#me1.1): Make SHA1 func

		//TODO: (#me1.2) Make HMAC func following AutoIT ones.
		//TODO (#me2): Give a look to bytebuffer (java)
	}
	return steamCode;
}

std::string generateSteamGuardCode(BYTE mSecret[])
{
	return generateSteamGuardCodeForTime(currentTime(), mSecret);
}

void hmacsha1(BYTE password[], BYTE message[])
{
	//--------------------------------------------------------------------
	// Declare variables.
	//
	// hProv:           Handle to a cryptographic service provider (CSP). 
	//                  This example retrieves the default provider for  
	//                  the PROV_RSA_FULL provider type.  
	// hHash:           Handle to the hash object needed to create a hash.
	// hKey:            Handle to a symmetric key. This example creates a 
	//                  key for the RC4 algorithm.
	// hHmacHash:       Handle to an HMAC hash.
	// pbHash:          Pointer to the hash.
	// dwDataLen:       Length, in bytes, of the hash.
	// password:           Password string used to create a symmetric key.
	// message:           Message string to be hashed.
	// HmacInfo:        Instance of an HMAC_INFO structure that contains 
	//                  information about the HMAC hash.
	// 
	HCRYPTPROV  hProv = NULL;
	HCRYPTHASH  hHash = NULL;
	HCRYPTKEY   hKey = NULL;
	HCRYPTHASH  hHmacHash = NULL;
	PBYTE       pbHash = NULL;
	DWORD       dwDataLen = 0;
	//BYTE        password[] = { 0x70,0x61,0x73,0x73,0x77,0x6F,0x72,0x64 };
	//BYTE        message[] = { 0x6D,0x65,0x73,0x73,0x61,0x67,0x65 };


	HMAC_INFO   HmacInfo;

	//--------------------------------------------------------------------
	// Zero the HMAC_INFO structure and use the SHA1 algorithm for
	// hashing.

	ZeroMemory(&HmacInfo, sizeof(HmacInfo));
	HmacInfo.HashAlgid = CALG_SHA1;

	//--------------------------------------------------------------------
	// Acquire a handle to the default RSA cryptographic service provider.

	if (!CryptAcquireContext(
		&hProv,                   // handle of the CSP
		NULL,                     // key container name
		NULL,                     // CSP name
		PROV_RSA_FULL,            // provider type
		CRYPT_VERIFYCONTEXT))     // no key access is requested
	{
		printf(" Error in AcquireContext 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	//--------------------------------------------------------------------
	// Derive a symmetric key from a hash object by performing the
	// following steps:
	//    1. Call CryptCreateHash to retrieve a handle to a hash object.
	//    2. Call CryptHashData to add a text string (password) to the 
	//       hash object.
	//    3. Call CryptDeriveKey to create the symmetric key from the
	//       hashed password derived in step 2.
	// You will use the key later to create an HMAC hash object. 

	if (!CryptCreateHash(
		hProv,                    // handle of the CSP
		CALG_SHA1,                // hash algorithm to use
		0,                        // hash key
		0,                        // reserved
		&hHash))                  // address of hash object handle
	{
		printf("Error in CryptCreateHash 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	if (!CryptHashData(
		hHash,                    // handle of the hash object
		password,                    // password to hash
		sizeof(password),            // number of bytes of data to add
		0))                       // flags
	{
		printf("Error in CryptHashData 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	if (!CryptDeriveKey(
		hProv,                    // handle of the CSP
		CALG_RC4,                 // algorithm ID
		hHash,                    // handle to the hash object
		0,                        // flags
		&hKey))                   // address of the key handle
	{
		printf("Error in CryptDeriveKey 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	//--------------------------------------------------------------------
	// Create an HMAC by performing the following steps:
	//    1. Call CryptCreateHash to create a hash object and retrieve 
	//       a handle to it.
	//    2. Call CryptSetHashParam to set the instance of the HMAC_INFO 
	//       structure into the hash object.
	//    3. Call CryptHashData to compute a hash of the message.
	//    4. Call CryptGetHashParam to retrieve the size, in bytes, of
	//       the hash.
	//    5. Call malloc to allocate memory for the hash.
	//    6. Call CryptGetHashParam again to retrieve the HMAC hash.

	if (!CryptCreateHash(
		hProv,                    // handle of the CSP.
		CALG_HMAC,                // HMAC hash algorithm ID
		hKey,                     // key for the hash (see above)
		0,                        // reserved
		&hHmacHash))              // address of the hash handle
	{
		printf("Error in CryptCreateHash 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	if (!CryptSetHashParam(
		hHmacHash,                // handle of the HMAC hash object
		HP_HMAC_INFO,             // setting an HMAC_INFO object
		(BYTE*)&HmacInfo,         // the HMAC_INFO object
		0))                       // reserved
	{
		printf("Error in CryptSetHashParam 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	if (!CryptHashData(
		hHmacHash,                // handle of the HMAC hash object
		message,                    // message to hash
		sizeof(message),            // number of bytes of data to add
		0))                       // flags
	{
		printf("Error in CryptHashData 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	//--------------------------------------------------------------------
	// Call CryptGetHashParam twice. Call it the first time to retrieve
	// the size, in bytes, of the hash. Allocate memory. Then call 
	// CryptGetHashParam again to retrieve the hash value.

	if (!CryptGetHashParam(
		hHmacHash,                // handle of the HMAC hash object
		HP_HASHVAL,               // query on the hash value
		NULL,                     // filled on second call
		&dwDataLen,               // length, in bytes, of the hash
		0))
	{
		printf("Error in CryptGetHashParam 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}

	pbHash = (BYTE*)malloc(dwDataLen);
	if (NULL == pbHash)
	{
		printf("unable to allocate memory\n");
		goto ErrorExit;
	}

	if (!CryptGetHashParam(
		hHmacHash,                 // handle of the HMAC hash object
		HP_HASHVAL,                // query on the hash value
		pbHash,                    // pointer to the HMAC hash value
		&dwDataLen,                // length, in bytes, of the hash
		0))
	{
		printf("Error in CryptGetHashParam 0x%08x \n", GetLastError());
		goto ErrorExit;
	}

	// Print the hash to the console.

	printf("The hash is:  ");
	for (DWORD i = 0; i < dwDataLen; i++)
	{
		printf("%2.2x ", pbHash[i]);
	}
	printf("\n");

	// Free resources.
ErrorExit:
	if (hHmacHash)
		CryptDestroyHash(hHmacHash);
	if (hKey)
		CryptDestroyKey(hKey);
	if (hHash)
		CryptDestroyHash(hHash);
	if (hProv)
		CryptReleaseContext(hProv, 0);
	if (pbHash)
		free(pbHash);
	//cin.get();
	//return 0;
}