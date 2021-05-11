#include "HttpRequest.h"
	
HttpRequest::HttpRequest(const char* date) {
	hOpenHandle = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hOpenHandle == NULL) {
		errorCode = -1;
		goto Exit;
	}
	if (hOpenHandle != nullptr) {
		url.append(uconv(date));
		hConnectHandle = InternetOpenUrl(hOpenHandle, url.c_str(), szHeaders, -1, INTERNET_FLAG_NO_AUTH | INTERNET_FLAG_NO_UI, INTERNET_NO_CALLBACK);
		if (hConnectHandle == NULL)
		{
			DWORD err = GetLastError();
			InternetCloseHandle(hOpenHandle);
			errorCode = -1;
			goto Exit;
		}
		char buf[1024];
		while (InternetReadFile(hConnectHandle, buf, (DWORD)sizeof(buf)-1, &numberOfBytesRead))
		{
			buf[numberOfBytesRead] = 0;
			responseData.append(buf);
			if (numberOfBytesRead < (DWORD)sizeof(buf)-1) 
				break;

		}
		if (!InternetCloseHandle(hConnectHandle) && !InternetCloseHandle(hOpenHandle)) {
			std::cout << GetLastError();
			errorCode = -1;
		}
	
	}
Exit:
	return;
}
