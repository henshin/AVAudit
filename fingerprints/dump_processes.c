#include <windows.h>
#include <tlhelp32.h>
void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    char buf[4096];
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    int index = 0;
    for(int i = Process32First(hProcess, &pe32); i; i=Process32Next(hProcess, &pe32)){
        char buf2[16] = {};
        wsprintfA(buf2, "%d ", pe32.th32ProcessID);
        int len = mystrlen(buf2);

        int length = mystrlen(pe32.szExeFile);
        memcpymyas(buf+index, buf2, len);
        memcpymyas(buf+index+len, pe32.szExeFile, length);
        memcpymyas(buf+index+len+length, "\n", 1);
        index += length+len+1;
    }


    for(int i = data[0]; i < data[1]; i++){
        if(buf[i] == '\0'){
            return 0;
        }
        leakResource(buf[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"