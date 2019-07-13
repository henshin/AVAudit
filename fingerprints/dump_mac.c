#include <windows.h>
#include <iphlpapi.h>

void leakResource(int num);
void* readConfig(void);
void * memcpymyas(void* dst, const void* src, unsigned int cnt);
size_t mystrlen(const char * str);

int WINAPI WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
    int* data = (int*)readConfig(); // first int is the start index, second is the end index

    /* BEGIN FINGERPRINTING */ 
    IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information 
                                           // for up to 16 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
   
    DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
      AdapterInfo,                 // [out] buffer to receive data
      &dwBufLen);                  // [in] size of receive data buffer
                                        // valid, no buffer overflow
   
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
                                                 // current adapter info
    char mac_addr[256];
    wsprintfA(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
    pAdapterInfo->Address[0], pAdapterInfo->Address[1],
        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
        pAdapterInfo->Address[4], pAdapterInfo->Address[5]); // Print MAC address    // Progress through 
                                            // linked liswhile(pAdapterInfo);     
    for(int i = data[0]; i < data[1]; i++){
        if(mac_addr[i] == '\0'){
            return 0;
        }
        leakResource(mac_addr[i]);
    }

    /* END FINGERPRINTING */

    ExitProcess(0);
    return 0;
}

// I can't seem to get mingw32-gcc to change the entrypoint of the binary (when compiling
// without the standard library. Hence, the entry point defaults to the first function
// so we must include at the bottom.
#include "leaker.h"


static void GetMACaddress(void)
{

}