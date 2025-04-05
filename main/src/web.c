#include <stdio.h>
#include <windows.h>
#include <winhttp.h>

unsigned char* fetch_url(const char* url);

int main() {
    fetch_url("http://localhost:3000/MessageBoxShellcode");
    /*printf("Response: ");
    for (int i = 0; i < 500; i++) { // For demonstration, printing first 100 bytes
        printf("%02x ", response[i]);
    }
    printf("\n");
    free(response);*/
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



    BOOL bResults = FALSE;
    HINTERNET hConnect = NULL, hRequest = NULL;


    hConnect = WinHttpConnect(hSession, host, port, 0);

    hRequest = WinHttpOpenRequest(hConnect, L"GET", path,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      0);
    if (hRequest)
    {
        DWORD dwFlags =
            SECURITY_FLAG_IGNORE_UNKNOWN_CA |
            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
        WinHttpSetOption(
            hRequest,
            WINHTTP_OPTION_SECURITY_FLAGS,
            &dwFlags,
            sizeof(dwFlags));

        bResults = WinHttpSendRequest(hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS,
                                      0, WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);
    }



unsigned char* response = malloc(10000);
    if (!response) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    size_t responseSize = 10000;
    size_t totalDownloaded = 0;

    if (hRequest && WinHttpReceiveResponse(hRequest, NULL)) {
        DWORD dwDownloaded, dwSize;
        do {
            // Get the size of the available data:
            // hRequest: request handle,
            // &dwSize: pointer to variable receiving size.
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize) || dwSize == 0)
                break;

            unsigned char* buf = malloc(dwSize);
            if (!buf)
                break;

            // Read the available data:
            // hRequest: request handle,
            // buf: buffer to store data,
            // dwSize: number of bytes to read,
            // &dwDownloaded: pointer to variable receiving number of bytes actually read.
            if (!WinHttpReadData(hRequest, (LPVOID)buf, dwSize, &dwDownloaded)) {
                free(buf);
                break;
            }

            // Reallocate the response buffer if needed.
            if (totalDownloaded + dwDownloaded > responseSize) {
                responseSize = totalDownloaded + dwDownloaded;
                unsigned char* temp = realloc(response, responseSize);
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
    } else {
        printf("Error %d has occurred.\n", GetLastError());
    }

    if (hRequest)
        WinHttpCloseHandle(hRequest);
    if (hConnect)
        WinHttpCloseHandle(hConnect);
    if (hSession)
        WinHttpCloseHandle(hSession);

    // If desired, you can realloc one more time to trim excess memory:
    unsigned char* finalResponse = realloc(response, totalDownloaded);
    for (size_t i=0; i<totalDownloaded; i++){printf("%c", finalResponse[i]);}
    return finalResponse ? finalResponse : response;
}