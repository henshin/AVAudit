#include <windows.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

#define NOP10() asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop")

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
	register int eax asm("eax");
	register int ebx asm("ebx");
	register int ecx asm("ecx");
	register int edx asm("edx");
	register int esi asm("esi");
	register int edi asm("edi");
	register int ebp asm("ebp");
	register int esp asm("esp");
	char buf1[512];
	char buf2[512];
	char final[1024];
	wsprintfA(buf1, "eax: %x\nebx: %x\necx: %x\nedx: %x\nesi: %x\nedi: %x\nebp: %x\nesp: %x\n\n", eax, ebx,
		ecx, edx, esi, edi, ebp, esp);
	NOP10();
	wsprintfA(buf2, "eax: %x\nebx: %x\necx: %x\nedx: %x\nesi: %x\nedi: %x\nebp: %x\nesp: %x\n", eax, ebx,
		ecx, edx, esi, edi, ebp, esp);

	int length = mystrlen(buf1);
	int len2 = mystrlen(buf2);
	memcpymyas(final, buf1, length);
	memcpymyas(final+length, buf2, len2);
	//MessageBoxA(0, buf2, 0, 0);

	for(int i = data[0]; i < data[1]; i++){
        if(final[i] == '\0'){
            return 0;
        }
        leakResource(final[i]);
    }
    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"