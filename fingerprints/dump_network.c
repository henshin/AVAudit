#include <windows.h>
#include <wininet.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    unsigned char cBuffer[1024];
    DWORD dwBytesRead;
    BOOL bResult;

    HINTERNET hInternet = NULL, hIConnect = NULL, hIUrlConnect = NULL;
    hInternet = InternetOpen(TEXT("Sample Application"),INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    hIUrlConnect = InternetOpenUrl(hInternet, TEXT("https://pastebin.com/raw/f53MJL1F"), NULL, 0, 0, 0);
    InternetReadFile(hIUrlConnect,(LPSTR)cBuffer, (DWORD)1024,&dwBytesRead);
    InternetCloseHandle(hIUrlConnect);
    InternetCloseHandle(hInternet);

    MessageBox(0, cBuffer, 0, 0);

    for(int i = data[0]; i < data[1]; i++){
        if(cBuffer[i] == '\0'){
            return 0;
        }
        leakResource(cBuffer[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"