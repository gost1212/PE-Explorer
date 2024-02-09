#include <windows.h>
#include <winternl.h>
#include <cstdio>
#include <strsafe.h>
/// @brief This function takes in {char*} that has the target file path and returns Heap HANDLE with the file data.
/// @param char*_filePath file path in {char*}
/// @return HANDLE: Returns an address in memory that holds the file content.
HANDLE GetFileContent(const char* lpFilePath){

    //Windows API function:
    //param[1]: Takes a {char*} which has a value of file path.
    //param[2]: Specifies read/write access type requested, can be GENERIC_READ, GENERIC_WRITE, or both by usin " | " in between. or supply 0 for neither.
    //param[3]: Specifies wither you'd give other processes can create new handles on the file you opened or not based on the sharing mode value you specify.
    //param[4]: Specifies wither children processes can inherited the created handle. pass "nullptr" to block inheritence.
    //param[5]: Specifies the action to make on target file, you can pass CREATE_ALWAYS, CREATE_NEW, OPEN_ALWAYS, OPEN_EXISTING, TRUNCATE_EXISTING as you please.
    //param[6]: Specifies the file attributes when searching for the target file (aka. hidden, system, temporary, etc).
    //param[7]: A template of file attributes that can be used when (if) creating a new file.
    const HANDLE hFile = CreateFileA(lpFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if(hFile == INVALID_HANDLE_VALUE){
        printf("An error occured while opening a file.\n");
        CloseHandle(hFile);
        return nullptr;
    }

    //Windows API function:
    //param[1]: Takes a file handle to read it's size.
    //param[2]: Takes a {LARGE_INTEGER} structure to save a high-order doubleWord value. it's only required if you're trying to a file that's larger than 4Gb in size.
    const DWORD_PTR dFileSize = GetFileSize(hFile, nullptr);
    if(dFileSize == INVALID_FILE_SIZE){
        printf("An error occured when trying to get file size.\n");
        CloseHandle(hFile);
        return nullptr;
    }
    printf("works\n");
    //Windows API function:
    //param[1]: Takes a HEAP handle to save the allocated memory into.
    //param[2]: Specifies a certain behavior when allocating memory.
    //param[2]: Takes the number of bytes to be allocated.
    const HANDLE hFileContent = HeapAlloc(GetProcessHeap(), 0, dFileSize);
    if(hFileContent == INVALID_HANDLE_VALUE){
        printf("An error occured when trying allocate memory to load file content.\n");
        CloseHandle(hFile);
        CloseHandle(hFileContent);
        return nullptr;
    }

    //Windows API function:
    //param[1]: Takes a FILE handle as file target.
    //param[2]: Takes a HEAP handle where the file content will be saved into.
    //param[3]: Specifies the number of bytes to read.
    //param[4]: Takes a pointer to variable that recieves read bytes when using synchronous FILE handle parameter. Can be nullptr.
    //param[5]: :P
    const BOOL bFileRead = ReadFile(hFile, hFileContent, dFileSize, nullptr, nullptr);
    if(!bFileRead){
        printf("An error occured when trying to read the file.\n");
        CloseHandle(hFile);
        if(hFileContent != nullptr){
            CloseHandle(hFileContent);
            return nullptr;
        }
    }

    CloseHandle(hFile);
    return hFileContent;
}


int main(const int argc, char* argv[]){
    
    char* lpFilePath;//prepare pointer to save file path

    if(argc < 2){//check if there's any passed arguments
        printf("Please pass some arguments\n");
        printf(argv[0]);
        return -1;
    }
    lpFilePath = argv[1];//take in passed file length
    printf(argv[0]);
    printf("\n");
    printf(argv[1]);
    printf("\n");
    
    const HANDLE hFileContent = GetFileContent(lpFilePath);
    if(hFileContent == INVALID_HANDLE_VALUE){//check if HANDLE is invalid
        if(hFileContent != nullptr){//if so, check if it's not a null pointer
            CloseHandle(hFileContent);//close handle
        }

        printf("Failed to read file.\n");
        return -1;
    }
    
    const PIMAGE_DOS_HEADER pImageDOSHeader = (PIMAGE_DOS_HEADER) hFileContent;//convert file data into DOS image type
    if(pImageDOSHeader == nullptr){//check if conversion has failed
        if(hFileContent != nullptr){//if so, check if content handle is a null pointer
            HeapFree(hFileContent, 0, nullptr);//free allocated heap memory
            CloseHandle(hFileContent);//close handle
        }
        printf("Failed to convert into DOS header.\n");
        return -1;
    }

    printf("Loaded file succesfully.\n");

    //identify phase
    int parseResult = 0;
    if(hFileContent != nullptr){
        HeapFree(hFileContent, 0, nullptr);
        CloseHandle(hFileContent);
    }
    return parseResult;
}