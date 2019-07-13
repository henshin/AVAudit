#include <windows.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);


int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    char result[512];
    EnumWindows(EnumWindowsProc, (LPARAM)result);
    for(int i = data[0]; i < data[1]; i++){
        if(result[i] == '\0'){
            return 0;
        }
        leakResource(result[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char name[80];
    static int index = 0;
    GetWindowText(hwnd,name,sizeof(name));
    int length = mystrlen(name);
    if (length == 0) return TRUE;
    memcpymyas((void*)(lParam+index), name, length);
    memcpymyas((void*)(lParam+index+length), "\n", 1);
    index += length+1;

    return TRUE;
}
// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"