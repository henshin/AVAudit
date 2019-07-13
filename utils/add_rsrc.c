#include <stdio.h>
#include <windows.h>
#include <iostream>

 void xor_crypt(char *data, int data_len)
{
    const char* key = "0vn203847n298374n28374n9287";

    for (int i = 0; i < data_len; i++)
        data[i] ^= key[ i % strlen(key) ];
}

void AddFileToExe(LPSTR lpExePath , LPSTR lpFilePath, int resource_number)
{
    FILE* fileptr = fopen(lpFilePath, "rb");
    fseek(fileptr, 0, SEEK_END);
    int dwFileSize = ftell(fileptr);
    char* lpBuffer  = new char[dwFileSize];

    rewind(fileptr);
    fread(lpBuffer, dwFileSize, 1, fileptr);
    fclose(fileptr);
    xor_crypt(lpBuffer, dwFileSize);

    HANDLE hResource = BeginUpdateResource(lpExePath ,FALSE);
    UpdateResource( hResource , RT_RCDATA , MAKEINTRESOURCE(resource_number), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpBuffer , dwFileSize);
    EndUpdateResource(hResource, FALSE);
    delete [] lpBuffer;
    CloseHandle(hResource);
}


int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 4){
        printf("%s [file stub] [directory/file path] {index (optional)}", argv[0]);
        return -1;
    }
    int counter = 1;
    if(argc == 3){
        WIN32_FIND_DATA ffd;
        char szDir[MAX_PATH];
        strcpy(szDir, argv[2]);
        strcat(szDir, "\\*");
        HANDLE hFind = FindFirstFileA(szDir, &ffd);
        
        if(INVALID_HANDLE_VALUE == hFind){
            printf("Could not recurse directory\n");
            return -1;
        }
        do {
            
            if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            }
            else {
                char filePath[MAX_PATH] = {0};
                wsprintfA(filePath, "%s%s", argv[2], ffd.cFileName);
                AddFileToExe(argv[1], filePath, counter);
                counter++;
            }
            
           /* if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                wsprintfA(filePath, "%s%s", argv[2], ffd.cFileName);
                printf("%s", filePath);
            } else {
                char filePath[MAX_PATH];
                wsprintfA(filePath, "%s%s", argv[2], ffd.cFileName);
                printf("%s", filePath);
                AddFileToExe(argv[1], filePath, counter);
               
            }*/
        } while (FindNextFileA(hFind, &ffd) != 0);
    } else if (argc == 4){
        AddFileToExe(argv[1], argv[2], atoi(argv[3]));
        printf("Added %s file to %s at index %d\n", argv[2], argv[1], atoi(argv[3]));
    }
    
    
    return 0;
}