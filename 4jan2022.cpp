#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <RASERROR.H>
#include <Winerror.h>
#include <ras.h>
#include <tchar.h>
#pragma comment(lib, "RASAPI32.LIB")

int main(int argc, char const *argv[])
{
	 HRESULT hr = NULL;
	 LPRASDIALPARAMS     lpRasDialParams = NULL;
	 LPRASENTRY pRasEntry = NULL;

	 

	 DWORD dwCb = 0;
	DWORD dwDevices = 0;
	LPRASDEVINFO lpRasDevInfo = NULL;
	DWORD dwRet = ERROR_SUCCESS;

	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);
	lpRasDevInfo = (LPRASDEVINFO) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
	lpRasDevInfo[0].dwSize = sizeof(RASDEVINFO);
	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);
	if (ERROR_SUCCESS == dwRet)
	{
		printf("The following RAS devices were found:\n");
	   
	  //  for(DWORD i = 0; i < dwDevices; i++){
			wprintf(L"%s: ", lpRasDevInfo[5].szDeviceName);
			wprintf(L"%s\n ", lpRasDevInfo[5].szDeviceType);
		// }
	}

	 DWORD cbb = sizeof(RASDIALPARAMS);
	lpRasDialParams = (LPRASDIALPARAMS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbb); 
	pRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RASENTRY));

	pRasEntry->dwSize = sizeof (RASENTRY);
	printf("HERE\n");
	pRasEntry->dwfOptions = RASEO_ModemLights | RASEO_NetworkLogon | RASEO_RequireEAP | RASEO_RequireMsCHAP2;
	pRasEntry->dwfNetProtocols = RASNP_Ip;
	pRasEntry->dwType = RASET_Vpn;
	pRasEntry->dwEncryptionType = ET_Require;
//	pRasEntry->dwVpnStrategy = VS_Ikev2Only; // THIS MADE RasSetEntryProperties() return an error.
	pRasEntry->dwFramingProtocol = RASFP_Ppp;  // ??????????????????
//	pRasEntry->dwfOptions2 = RASEO2_RequireMachineCertificates;
	
	hr = StringCchCopy(pRasEntry->szLocalPhoneNumber, ARRAYSIZE(pRasEntry->szLocalPhoneNumber), L"192.168.1.6");
	if(FAILED(hr))
	{
		printf("FAILED hr 1\n");
	}

	hr = StringCchCopy(pRasEntry->szDeviceName, ARRAYSIZE(pRasEntry->szDeviceName), lpRasDevInfo[5].szDeviceName);
	if(FAILED(hr))
	{
		printf("FAILED hr 2\n");
	}

	hr = StringCchCopy(pRasEntry->szDeviceType, ARRAYSIZE(pRasEntry->szDeviceType), lpRasDevInfo[5].szDeviceType);
	if(FAILED(hr))
	{
		printf("FAILED hr 3\n");
	}

	

	DWORD dwErr = RasSetEntryProperties(NULL,
		L"myVpnEntry",
		pRasEntry,
		sizeof (RASENTRY),
		NULL,
		0
		);

	if(dwErr == ERROR_SUCCESS)
	{
		printf("RasSetEntryProperties Successful\n");
	}else{
		printf("RasSetEntryProperties Failed %d\n", dwErr);

		if(dwErr == ERROR_ACCESS_DENIED)
		{
			printf("dwErr == ERROR_ACCESS_DENIED\n");
		}
		if(dwErr == ERROR_BUFFER_INVALID)
		{
			printf("dwErr == ERROR_BUFFER_INVALID\n");
		}

		if(dwErr == ERROR_CANNOT_OPEN_PHONEBOOK)
		{
			printf("dwErr == ERROR_CANNOT_OPEN_PHONEBOOK\n");
		}

		if(dwErr == ERROR_INVALID_PARAMETER)
		{
			printf("dwErr == ERROR_INVALID_PARAMETER\n");
		}
		
	}

	

	lpRasDialParams->dwSize = sizeof(RASDIALPARAMS);

	 StringCchCopy(lpRasDialParams->szPhoneNumber, ARRAYSIZE(lpRasDialParams->szPhoneNumber), L"");
	if(FAILED(hr))
	{
		printf("FAILED StringCchCopy(lpRasDialParams->szPhoneNumber...\n");
	}

	StringCchCopy(lpRasDialParams->szEntryName, ARRAYSIZE(lpRasDialParams->szEntryName), L"myVpnEntry");
	if(FAILED(hr))
	{
		printf("FAILED StringCchCopy(lpRasDialParams->szEntryName...\n");
	}

	StringCchCopy(lpRasDialParams->szCallbackNumber, ARRAYSIZE(lpRasDialParams->szCallbackNumber), (STRSAFE_LPCWSTR)"");
	if(FAILED(hr))
	{
		printf("FAILED StringCchCopy(lpRasDialParams->szCallbackNumber...\n");
	}

	 StringCchCopy(lpRasDialParams->szUserName, ARRAYSIZE(lpRasDialParams->szUserName), L"hman");
	if(FAILED(hr))
	{
		printf("FAILED StringCchCopy(lpRasDialParams->szUserName...\n");
	}

	StringCchCopy(lpRasDialParams->szPassword, ARRAYSIZE(lpRasDialParams->szPassword), L"wahgwan");
	if(FAILED(hr))
	{
		printf("FAILED StringCchCopy(lpRasDialParams->szPassword\n");
	} 

	HRASCONN hRasConn = NULL;

	dwErr = RasDial(NULL,
		NULL,
		lpRasDialParams,
		NULL,
		NULL,
		&hRasConn
		);

	if(dwErr == ERROR_SUCCESS)
	{
		printf("RasDial Successful\n");
	}else{
		printf("RasDial Failed %d\n", dwErr);
		RasHangUp(hRasConn);
	}

	if(hRasConn == NULL)
	{
		printf("HRASCONN == NULL\n");
	}


	// RasSetCredentialsA function (ras.h)
	// RASCREDENTIALS structure (Windows)
	// RASEO_NetworkLogon


	

	return 0;
}