#include <winnt.h>
#include <windows.h>
size_t mystrlen(const char * str);
void leakResource(int num);
void* readConfig(void);
void* memcpymyas(void* dst, const void* src, unsigned int cnt);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */
   // int pi;
    //asm("fldpi" : "=t" (pi));
    int pi = 3;
    char pis[64];
    wsprintfA(pis, "%d", pi);
    for(int i = data[0]; i < data[1]; i++){
        if(pis[i] == '\0'){
            return 0;
        }
        leakResource(pis[i]);
    }
    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"


