#include <getopt.h>
#include "loadFileContent.cpp"

const char* helpMessage =
"-e   Print exported functions only\n"
"-f   Print imported functions only\n"
"-l   Print imported libraries only\n";

HANDLE GetFileContent(const char* lpFilePath);

static struct option long_options[] = {
    {"exported-functions", no_argument, 0, 0},
    {"import-functions", no_argument, 0, 0},
    {"import-libraries", no_argument, 0, 0},
    {"help", no_argument, 0, 0}
};

void executeLongOption(const char* lOption){
    printf("nooo\n");
}

int main(const int argc, char* argv[]){
    
    char* c_lpFilePath = NULL;//prepare pointer to save file path

    if(argc < 2){//check if there's any passed arguments
        printf("Please pass some arguments\n");
        return -1;
    }
    bool b_printAll = true;
    bool b_importedFunctions = false;
    bool b_importedLibraries = false;
    bool b_exportedFunctions = false;
    int l_option_index = 0;

    int c;
    while((c = getopt_long(argc, argv, ":i:eflh", long_options, &l_option_index)) != -1){
        switch(c){
            case 0:
                executeLongOption(long_options[l_option_index].name);
                break;
            case 'e':
                printf("Exported functions only\n");
                b_exportedFunctions = true;
                b_printAll = false;
                break;
            case 'f':
                printf("Imported functions only\n");
                b_importedFunctions = true;
                b_printAll = false;
                break;
            case 'l':
                printf("Imported libraries only\n");
                b_importedLibraries = true;
                b_printAll = false;
                break;
            case 'h':
                printf(helpMessage);
                return 0;
            case 'i':
                c_lpFilePath = optarg;
                break;
            case '?':
                printf(helpMessage);
                return 0;
            default:
                printf("Invalid arguments\n");
                return 0;
        }
    }

    if(c_lpFilePath == NULL){//check if file path is passed
        printf("Please pass file path\n");
        return -1;
    }
    
    const HANDLE hFileContent = GetFileContent(c_lpFilePath);
    if(hFileContent == INVALID_HANDLE_VALUE){//check if HANDLE is invalid
        if(hFileContent != nullptr){//if so, check if it's not a null pointer
            CloseHandle(hFileContent);//close handle
        }

        printf("Failed to read file.\n");
        return -1;
    }
    printf("Loaded file succesfully.\n");

    const PIMAGE_DOS_HEADER pImageDOSHeader = (PIMAGE_DOS_HEADER) hFileContent;//convert file data into DOS image type
    if(pImageDOSHeader == nullptr){//check if conversion has failed
        if(hFileContent != nullptr){//if so, check if content handle is a null pointer
            HeapFree(hFileContent, 0, nullptr);//free allocated heap memory
            CloseHandle(hFileContent);//close handle
        }
        printf("Failed to convert into DOS header.\n");
        return -1;
    }
    printf("Parsed DOS header succesfully.\n");

    const PIMAGE_NT_HEADERS pImageNTHeaders = (PIMAGE_NT_HEADERS) ((DWORD_PTR) pImageDOSHeader + pImageDOSHeader->e_lfanew);//convert
    if (pImageNTHeaders == nullptr){
        if (hFileContent != nullptr){
            HeapFree(hFileContent, 0, nullptr);
            CloseHandle(hFileContent);
        }
        return -1;
    }
    printf("Parsed NT header succesfully.\n");

    //identify phase
    int magicNumber = pImageNTHeaders->OptionalHeader.Magic;
    if(magicNumber == IMAGE_NT_OPTIONAL_HDR32_MAGIC){

    }else if(magicNumber == IMAGE_NT_OPTIONAL_HDR64_MAGIC){

    }else{
        printf("Invalid magic number\n");
        return -1;
    }
    //closing phase
    if(hFileContent != nullptr){
        HeapFree(hFileContent, 0, nullptr);
        CloseHandle(hFileContent);
        return 0;
    }
    printf("Please pass correct arguments\n\n");
    printf(helpMessage);
    return 0;
}