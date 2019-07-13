#include <windows.h>

unsigned long __readfsdword(unsigned long Offset);
void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

typedef struct _PEB_LDR_DATA {
    UINT8 _PADDING_[12];
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;


typedef struct _PEB {
    UINT8 _PADDING_[24];
    UINT8 _PADDING2[12];
    PEB_LDR_DATA* Ldr;
} PEB, *PPEB;



int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    unsigned char all_bytes[256];

    BYTE* _teb = (BYTE*)__readfsdword(0x18);
    PEB* _peb = *(PEB**)(_teb+0x30);
    DWORD imageBase = (DWORD)_peb->Ldr->InMemoryOrderModuleList.Flink + 0x10;
    void *vp2 = *(void**)imageBase;
    leakResource(64);
    IMAGE_DOS_HEADER * dos = (IMAGE_DOS_HEADER*)vp2;
    IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS * )((DWORD)vp2+dos->e_lfanew);
    if(nth->Signature == IMAGE_NT_SIGNATURE){
        leakResource(65);
    } else {
        leakResource(66);
        leakResource(66);
    }
    ExitProcess(0);
    
    for (int i = 1; i < 256; i++){
        all_bytes[i-1] = i;
    }
    for(int i = data[0]; i < data[1]; i++){
        if(all_bytes[i] == '\0'){
            return 0;
        }
        
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

/* for x86 only */
unsigned long __readfsdword(unsigned long Offset)
{
   unsigned long ret;
   __asm__ volatile ("movl  %%fs:%1,%0"
     : "=r" (ret) ,"=m" ((*(volatile long *) Offset)));
   return ret;
}
// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"