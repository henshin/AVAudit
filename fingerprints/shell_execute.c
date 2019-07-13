#include <windows.h>

void myzeromem(char* src, size_t length);
void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;
    if(!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        MessageBoxA(0, "cant create pipe", 0, 0);

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        MessageBoxA(0, "cant set handle information", 0, 0);

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;

    myzeromem((char*)&piProcInfo, sizeof(PROCESS_INFORMATION));
    myzeromem((char*)&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    if(!CreateProcessA(NULL, "C:\\Windows\\System32\\cmd.exe /C echo hi", NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)){
        MessageBoxA(0, "couldn't open process", 0, 0);
    }
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(g_hChildStd_OUT_Wr);
    char outBuf[4096];
    myzeromem((char*)outBuf, 4096);
    CHAR chBuf[4096];
    DWORD dwRead;
    int index =0;
    while(ReadFile(g_hChildStd_OUT_Rd, chBuf, 4096, &dwRead, NULL)){
        char buf[10];
        wsprintfA(buf, "%d", dwRead);
        if(dwRead == 0){
            break;
        }
        memcpymyas(outBuf+index, chBuf, dwRead);
        index += dwRead;
        dwRead = 0;
    }
    
    for(int i = data[0]; i < data[1]; i++){
        if(outBuf[i] == '\0'){
            ExitProcess(0);
            return 0;
        }
        leakResource(outBuf[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"