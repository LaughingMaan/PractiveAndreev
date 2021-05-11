#pragma once

#include "Logs.h"
#include <string>
#include <tchar.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet")
using ns::Utils;
#ifdef UNICODE
#define mystring  wstring
#define uconv(v) Utils::ansi2unicode(v)
#else
#define mystring  string
#define uconv(v) Utils::unicode2ansi(v)
#endif // !UNICODE
class HttpRequest
{
private:
	std::mystring url = _T("http://www.dsdev.tech/logs/");
	HINTERNET hOpenHandle,hConnectHandle = NULL;
	const TCHAR* szHeaders = _T("Content-Type:application/json; charset=utf-8\r\n");
	std::string buffer = "";
	DWORD numberOfBytesRead = 0;
public:	
	std::string responseData = "";
	int errorCode = 0;
	HttpRequest(const char* date);
};

