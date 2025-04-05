#include <windows.h>
#include <winhttp.h>
#include <malloc.h>
#include "webreq.h"

int main() {
    unsigned char* response = fetch_url("http://localhost:3000/MessageBoxShellcode");
    printf("Response: ");
    for (int i = 0; i < 500; i++) { // For demonstration, printing first 100 bytes
        printf("%02x ", response[i]);
    }
    printf("\n");

    // Don't forget to free the allocated memory
    free(response);
}

unsigned char* fetch_url(const char* url) {
    WCHAR wUrl[100];
    MultiByteToWideChar(CP_UTF8, 0, url, (int)strlen(url), wUrl, 100);

    HINTERNET hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     NULL, NULL, 0);

    URL_COMPONENTS urlComp;
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    urlComp.dwSchemeLength    = (DWORD)-1;
    urlComp.dwHostNameLength  = (DWORD)-1;
    urlComp.dwUrlPathLength   = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    WinHttpCrackUrl(wUrl, (DWORD)wcslen(wUrl), 0, &urlComp);
   
    LPWSTR hostlong = urlComp.lpszHostName;
    LPCWSTR host = wcstok(hostlong, L":");
    INTERNET_PORT port = urlComp.nPort;
    LPWSTR path = urlComp.lpszUrlPath;
    printf("%ls, %d, %ls\n", host, port, path);
   
    HINTERNET hConnect = WinHttpConnect(hSession, host, port, 0);

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path, NULL, NULL, NULL, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
       printf("%lu", GetLastError());
       WinHttpCloseHandle(hConnect);
       WinHttpCloseHandle(hSession);
       return NULL;
    }
    WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0);
    WinHttpReceiveResponse(hRequest, NULL);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    unsigned char* response = (unsigned char*)malloc(10000);  // Allocate space for response
    memset(response, 0, 10000);  // Initialize memory to zero

    unsigned char* currentPosition = response;  // Pointer to track where to append data

    do {
        dwSize = 0;
        WinHttpQueryDataAvailable(hRequest, &dwSize);

        unsigned char* pszOutBuffer = (unsigned char*)malloc(dwSize);
        if (pszOutBuffer) {
            ZeroMemory(pszOutBuffer, dwSize);
            WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded);

            // Append to the response buffer (raw byte data, no string operations)
            memcpy(currentPosition, pszOutBuffer, dwDownloaded);
            currentPosition += dwDownloaded;  // Move pointer forward
            free(pszOutBuffer);
        }
    } while (dwSize > 0);

    // Cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return response;  // Return the raw byte array
}

