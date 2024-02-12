#include <windows.h>
#include <winternl.h>
#include <cstdio>
#include <strsafe.h>
/// @brief This function takes in {char*} that has the target file path and returns Heap HANDLE with the file data.
/// @param char*_filePath file path in {char*}
/// @return HANDLE: Returns an address in memory that holds the file content.
HANDLE GetFileContent(const char* lpFilePath){
    //open target file
    const HANDLE hFile = CreateFileA(lpFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if(hFile == INVALID_HANDLE_VALUE){
        printf("An error occured while opening a file.\n");
        CloseHandle(hFile);
        return nullptr;
    }
    //get target file size
    const DWORD_PTR dFileSize = GetFileSize(hFile, nullptr);
    if(dFileSize == INVALID_FILE_SIZE){
        printf("An error occured when trying to get file size.\n");
        CloseHandle(hFile);
        return nullptr;
    }
    //allocate memory in the same size as the file's size
    const HANDLE hFileContent = HeapAlloc(GetProcessHeap(), 0, dFileSize);
    if(hFileContent == INVALID_HANDLE_VALUE){
        printf("An error occured when trying allocate memory to load file content.\n");
        CloseHandle(hFile);
        CloseHandle(hFileContent);
        return nullptr;
    }
    //copy file content to the allocated memory
    const BOOL bFileRead = ReadFile(hFile, hFileContent, dFileSize, nullptr, nullptr);
    if(!bFileRead){
        printf("An error occured when trying to read the file.\n");
        CloseHandle(hFile);
        if(hFileContent != nullptr){
            CloseHandle(hFileContent);
            return nullptr;
        }
    }
    //close file handle
    CloseHandle(hFile);
    return hFileContent;//return heap handle
}