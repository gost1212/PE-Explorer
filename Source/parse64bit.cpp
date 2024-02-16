#include "includes.hpp"
#include "flags.hpp"

/**
 * Function wich parse x64 PE file.
 * \param pImageDOSHeader : pointer of the DOS header of the PE file. 
 * \param flags : flags to filter the output.
 * \return : 0 if the parsing is succeful else -1.
 */
int ParseImage64(PIMAGE_DOS_HEADER pImageDOSHeader, uint16_t flags){
    const PIMAGE_NT_HEADERS64 pImageNTHeader64 = (PIMAGE_NT_HEADERS64)((DWORD_PTR)pImageDOSHeader + pImageDOSHeader->e_lfanew);

    if(pImageNTHeader64 == nullptr) return -1;

    const IMAGE_FILE_HEADER ImageFileHeader = pImageNTHeader64->FileHeader;
    const IMAGE_OPTIONAL_HEADER64 ImageOptionalHeader64 = pImageNTHeader64->OptionalHeader;

    const PIMAGE_SECTION_HEADER pImageSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)pImageNTHeader64 + 4 + sizeof(IMAGE_FILE_HEADER) + ImageFileHeader.SizeOfOptionalHeader);
    if(pImageSectionHeader == nullptr) return -1;
    
    if(flags & PRINTALL){
        printf("PE IMAGE INFORMATION:\n\n");
        printf("Architecture: x64\n");
        printf("\nDOS HEADER:\n");
        printf("e_magic: 0x%X\n", (uintptr_t)pImageDOSHeader->e_magic);
        printf("e_cblp: 0x%X\n", (uintptr_t)pImageDOSHeader->e_cblp);
        printf("e_cp: 0x%X\n", (uintptr_t)pImageDOSHeader->e_cp);
        printf("e_crlc: 0x%X\n", (uintptr_t)pImageDOSHeader->e_crlc);
        printf("e_cparhdr: 0x%X\n", (uintptr_t)pImageDOSHeader->e_cparhdr);
        printf("e_minalloc: 0x%X\n", (uintptr_t)pImageDOSHeader->e_minalloc);
        printf("e_maxalloc: 0x%X\n", (uintptr_t)pImageDOSHeader->e_maxalloc);
        printf("e_ss: 0x%X\n", (uintptr_t)pImageDOSHeader->e_ss);
        printf("e_sp: 0x%X\n", (uintptr_t)pImageDOSHeader->e_sp);
        printf("e_csum: 0x%X\n", (uintptr_t)pImageDOSHeader->e_csum);
        printf("e_ip: 0x%X\n", (uintptr_t)pImageDOSHeader->e_ip);
        printf("e_cs: 0x%X\n", (uintptr_t)pImageDOSHeader->e_cs);
        printf("e_lfarlc: 0x%X\n", (uintptr_t)pImageDOSHeader->e_lfarlc);
        printf("e_ovno: 0x%X\n", (uintptr_t)pImageDOSHeader->e_ovno);
        printf("e_oemid: 0x%X\n", (uintptr_t)pImageDOSHeader->e_oemid);
        printf("e_oeminfo: 0x%X\n", (uintptr_t)pImageDOSHeader->e_oeminfo);
        printf("e_lfanew: 0x%X\n", (uintptr_t)pImageDOSHeader->e_lfanew);
    }

    return 0;
}