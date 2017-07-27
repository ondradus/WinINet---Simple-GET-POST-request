#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <WinInet.h>
#include <iostream>
#include <string>



#define  INTERNET_OPEN_TYPE_DIRECT 1
#define  INTERNET_SERVICE_HTTP 3
#define GET 0
#define POST 1

/*
____ _ _  _ ___  _    ____    ____ ____ ___   / ___  ____ ____ ___
[__  | |\/| |__] |    |___    | __ |___  |   /  |__] |  | [__   |
___] | |  | |    |___ |___    |__] |___  |  /   |    |__| ___]  |

 * A very simple example of using WinINet for GET/POST requests in C++.
 * Ideal for C++ beginners who can't get curl or winsock to work and are desperate to make HTTP requests.
 * Stop tearing your hair out and use this :p

Author: Dondor17

*/
/*
---------------- BEGN TODO ----------------

-> Choose  GET/POST
-> Function to convert from str to const wchar_t

---------------- END TODO ----------------
*/

#pragma comment (lib, "wininet.lib")
using namespace std;


string HttpRequest(string site, string param);

int main(int argc, char *argv[])
{
	string my_response = HttpRequest("127.0.0.1", "test.php");
	cout << my_response << std::endl;

	system("PAUSE");
	return 0;
}




	//Our lovely function
	string HttpRequest(string site, string param) {
	HINTERNET hInternet = InternetOpenW(L"YourUserAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); //you should perhaps edit your useragent ? :p

	if (hInternet == NULL)
	{
		return "InternetOpenW failed(hInternet): " + GetLastError();
	}
	else
	{
		wstring widestr;
		for (int i = 0; i < site.length(); ++i)
		{
			widestr += wchar_t(site[i]);
		}
		const wchar_t* site_name = widestr.c_str();
		
		wstring widestr2;
		for (int i = 0; i < param.length(); ++i)
		{
			widestr2 += wchar_t(param[i]);
		}
		const wchar_t* site_param = widestr2.c_str();

		// We need to convert str to const wchar_t as the args require!

		HINTERNET hConnect = InternetConnectW(hInternet,site_name, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

		if (hConnect == NULL)
		{
			return "InternetConnectW failed(hConnect == NULL): " + GetLastError();
		}
		else
		{
			const wchar_t* parrAcceptTypes[] = { L"text/*", NULL }; // accepted types. We'll choose text.

			HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", site_param, NULL, NULL, parrAcceptTypes, 0, 0);

			if (hRequest == NULL)
			{
				return "HttpOpenRequestW failed(hRequest == NULL): " + GetLastError();
			}
			else
			{
				BOOL bRequestSent = HttpSendRequestW(hRequest, NULL, 0, NULL, 0);

				if (!bRequestSent)
				{
					return "!bRequestSent    HttpSendRequestW failed with error code " + GetLastError();				
				}
				else
				{
					std::string strResponse;
					const int nBuffSize = 1024;
					char buff[nBuffSize];

					BOOL bKeepReading = true;
					DWORD dwBytesRead = -1;

					while (bKeepReading && dwBytesRead != 0)
					{
						bKeepReading = InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
						strResponse.append(buff, dwBytesRead);
					}
					return strResponse;
				}
				InternetCloseHandle(hRequest);
			}
			InternetCloseHandle(hConnect);
		}
		InternetCloseHandle(hInternet);
	}
}