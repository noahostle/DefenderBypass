#include <stdio.h>
#include <windows.h>
#include <winhttp.h>
#include "webreq.h"

void fetch_url(const char* url, unsigned char* response, size_t* responseSize) {
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



    BOOL bResults = FALSE;
    HINTERNET hConnect = NULL, hRequest = NULL;


    hConnect = WinHttpConnect(hSession, host, port, 0);

    hRequest = WinHttpOpenRequest(hConnect, L"GET", path,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      0);
    if (hRequest) {
        bResults = WinHttpSendRequest(hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS,
                                      0, WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);
    }



    size_t totalDownloaded = 0;

    if (hRequest && WinHttpReceiveResponse(hRequest, NULL)) {
        DWORD dwDownloaded, dwSize;
        do {
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize) || dwSize == 0)
                break;

            unsigned char* buf = malloc(dwSize);
            if (!buf)
                break;

            if (!WinHttpReadData(hRequest, (LPVOID)buf, dwSize, &dwDownloaded)) {
                free(buf);
                break;
            }

            if (totalDownloaded + dwDownloaded > *responseSize) {
                *responseSize = totalDownloaded + dwDownloaded;
                unsigned char* temp = realloc(response, *responseSize);
                if (!temp) {
                    free(buf);
                    break;
                }
                response = temp;
            }
            memcpy(response + totalDownloaded, buf, dwDownloaded);
            totalDownloaded += dwDownloaded;
            free(buf);

        } while (dwSize > 0);
    }

    if (hRequest)
        WinHttpCloseHandle(hRequest);
    if (hConnect)
        WinHttpCloseHandle(hConnect);
    if (hSession)
        WinHttpCloseHandle(hSession);

}
