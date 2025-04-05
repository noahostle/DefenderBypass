#include <windows.h>
#include <winhttp.h>
#include <string.h>
#include <malloc.h>
#include "webreq.h"

char* fetch_url(const char* url) {
    HINTERNET hSession = WinHttpOpen(L"SimpleClient", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     NULL, NULL, 0);
    if (!hSession) return NULL;

    // Extract just the host from the URL (e.g., "www.example.com")
    const char* host_start = strstr(url, "://");
    const char* host = host_start ? host_start + 3 : url; // Skip "https://"

    WCHAR wHost[256];
    MultiByteToWideChar(CP_UTF8, 0, host, -1, wHost, sizeof(wHost)/sizeof(WCHAR));

    // Set the port to the default HTTPS port
    HINTERNET hConnect = WinHttpConnect(hSession, wHost, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return NULL;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL, NULL, NULL, NULL, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return NULL;
    }

    WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0);
    WinHttpReceiveResponse(hRequest, NULL);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    char* response = (char*)malloc(10000);
    memset(response, 0, 10000);

    do {
        dwSize = 0;
        WinHttpQueryDataAvailable(hRequest, &dwSize);

        pszOutBuffer = (LPSTR) malloc(dwSize + 1);
        if (pszOutBuffer) {
            ZeroMemory(pszOutBuffer, dwSize + 1);
            WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded);
            strcat(response, pszOutBuffer);  // Append to the response buffer
            free(pszOutBuffer);
        }
    } while (dwSize > 0);

    // Cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return response;
}
