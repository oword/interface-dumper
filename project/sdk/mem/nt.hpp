//definitions for some internal structures
#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace nt
{

//0x14 bytes (sizeof)
    struct _IMAGE_FILE_HEADER
    {
        USHORT Machine;                                                         //0x0
        USHORT NumberOfSections;                                                //0x2
        ULONG TimeDateStamp;                                                    //0x4
        ULONG PointerToSymbolTable;                                             //0x8
        ULONG NumberOfSymbols;                                                  //0xc
        USHORT SizeOfOptionalHeader;                                            //0x10
        USHORT Characteristics;                                                 //0x12
    };

    typedef struct _IMAGE_EXPORT_DIRECTORY {
        DWORD Characteristics;
        DWORD TimeDateStamp;
        WORD MajorVersion;
        WORD MinorVersion;
        DWORD Name;
        DWORD ordinal_Base;
        DWORD NumberOfFunctions;
        DWORD NumberOfNames;
        DWORD AddressOfFunctions;    //RVA
        DWORD AddressOfNames;        //RVA
        DWORD AddressOfNameOrdinals; //RVA
    } IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

//0x8 bytes (sizeof)
    struct _IMAGE_DATA_DIRECTORY
    {
        DWORD VirtualAddress;                                                   //0x0
        DWORD Size;                                                             //0x4
    };

//0xe0 bytes (sizeof)
    struct _IMAGE_OPTIONAL_HEADER
    {
        USHORT Magic;                                                           //0x0
        UCHAR MajorLinkerVersion;                                               //0x2
        UCHAR MinorLinkerVersion;                                               //0x3
        ULONG SizeOfCode;                                                       //0x4
        ULONG SizeOfInitializedData;                                            //0x8
        ULONG SizeOfUninitializedData;                                          //0xc
        ULONG AddressOfEntryPoint;                                              //0x10
        ULONG BaseOfCode;                                                       //0x14
        ULONG BaseOfData;                                                       //0x18
        ULONG ImageBase;                                                        //0x1c
        ULONG SectionAlignment;                                                 //0x20
        ULONG FileAlignment;                                                    //0x24
        USHORT MajorOperatingSystemVersion;                                     //0x28
        USHORT MinorOperatingSystemVersion;                                     //0x2a
        USHORT MajorImageVersion;                                               //0x2c
        USHORT MinorImageVersion;                                               //0x2e
        USHORT MajorSubsystemVersion;                                           //0x30
        USHORT MinorSubsystemVersion;                                           //0x32
        ULONG Win32VersionValue;                                                //0x34
        ULONG SizeOfImage;                                                      //0x38
        ULONG SizeOfHeaders;                                                    //0x3c
        ULONG CheckSum;                                                         //0x40
        USHORT Subsystem;                                                       //0x44
        USHORT DllCharacteristics;                                              //0x46
        ULONG SizeOfStackReserve;                                               //0x48
        ULONG SizeOfStackCommit;                                                //0x4c
        ULONG SizeOfHeapReserve;                                                //0x50
        ULONG SizeOfHeapCommit;                                                 //0x54
        ULONG LoaderFlags;                                                      //0x58
        ULONG NumberOfRvaAndSizes;                                              //0x5c
        struct _IMAGE_DATA_DIRECTORY DataDirectory[16];                         //0x60
    };

    //0xf0 bytes (sizeof)
    struct _IMAGE_OPTIONAL_HEADER64
    {
        USHORT Magic;                                                           //0x0
        UCHAR MajorLinkerVersion;                                               //0x2
        UCHAR MinorLinkerVersion;                                               //0x3
        ULONG SizeOfCode;                                                       //0x4
        ULONG SizeOfInitializedData;                                            //0x8
        ULONG SizeOfUninitializedData;                                          //0xc
        ULONG AddressOfEntryPoint;                                              //0x10
        ULONG BaseOfCode;                                                       //0x14
        ULONGLONG ImageBase;                                                    //0x18
        ULONG SectionAlignment;                                                 //0x20
        ULONG FileAlignment;                                                    //0x24
        USHORT MajorOperatingSystemVersion;                                     //0x28
        USHORT MinorOperatingSystemVersion;                                     //0x2a
        USHORT MajorImageVersion;                                               //0x2c
        USHORT MinorImageVersion;                                               //0x2e
        USHORT MajorSubsystemVersion;                                           //0x30
        USHORT MinorSubsystemVersion;                                           //0x32
        ULONG Win32VersionValue;                                                //0x34
        ULONG SizeOfImage;                                                      //0x38
        ULONG SizeOfHeaders;                                                    //0x3c
        ULONG CheckSum;                                                         //0x40
        USHORT Subsystem;                                                       //0x44
        USHORT DllCharacteristics;                                              //0x46
        ULONGLONG SizeOfStackReserve;                                           //0x48
        ULONGLONG SizeOfStackCommit;                                            //0x50
        ULONGLONG SizeOfHeapReserve;                                            //0x58
        ULONGLONG SizeOfHeapCommit;                                             //0x60
        ULONG LoaderFlags;                                                      //0x68
        ULONG NumberOfRvaAndSizes;                                              //0x6c
        struct _IMAGE_DATA_DIRECTORY DataDirectory[16];                         //0x70
    };

#ifdef _WIN64
//0x108 bytes (sizeof)
    typedef struct _IMAGE_NT_HEADERS64
    {
        DWORD Signature;                                                        //0x0
        struct _IMAGE_FILE_HEADER FileHeader;                                           //0x4
        struct _IMAGE_OPTIONAL_HEADER64 OptionalHeader;                                 //0x18
    } PE_HEADER;
#else
    //0xf8 bytes (sizeof)
typedef struct _IMAGE_NT_HEADERS
{
    DWORD Signature;                                                        //0x0
    struct _IMAGE_FILE_HEADER FileHeader;                                           //0x4
    struct _IMAGE_OPTIONAL_HEADER OptionalHeader;                                   //0x18
} PE_HEADER;
#endif

//0x40 bytes (sizeof)
    typedef struct _IMAGE_DOS_HEADER
    {
        USHORT e_magic;            //0x0
        char pad[0x3a];             //0x2
        LONG e_lfanew;              //0x3c
    } DOS_HEADER;

    typedef struct _UNICODE_STRING {
        USHORT Length;
        USHORT MaximumLength;
        wchar_t*  Buffer;
    } UNICODE_STRING;

    typedef struct _LIST_ENTRY {
        struct _LIST_ENTRY *Flink;
        struct _LIST_ENTRY *Blink;
    } LIST_ENTRY, *PLIST_ENTRY;

    typedef struct _LDR_DATA_TABLE_ENTRY {
        LIST_ENTRY InLoadOrderLinks;
        PVOID Reserved1;
        LIST_ENTRY InMemoryOrderLinks;
        PVOID Reserved2[2];
        PVOID DllBase;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        PVOID Reserved5[3];
        union {
            ULONG CheckSum;
            PVOID Reserved6;
        };
        ULONG TimeDateStamp;
    } LDR_DATA_TABLE_ENTRY, LDR_DATA, *PLDR_DATA_TABLE_ENTRY, *PLDR_DATA;

    typedef struct PEB_LDR_DATA {
        ULONG         Length;
        BOOL   Initialized;
        PVOID      SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
    } PEB_LDR_DATA, *PPEB_LDR_DATA;

    typedef struct _PEB {
        BYTE                         Reserved1[2];
        BYTE                          BeingDebugged;
        BYTE                          Reserved2[1];
        PVOID                         Reserved3[2];
        PPEB_LDR_DATA                 Ldr;
        PVOID                         ProcessParameters;
        PVOID                        Reserved4[3];
        PVOID                         Reserved5;
        PVOID                         AtlThunkSListPtr;
        ULONG                        Reserved6;
        PVOID                        Reserved7;
        ULONG                         Reserved8;
        ULONG                        AtlThunkSListPtr32;
        PVOID                        Reserved9[45];
        BYTE                          Reserved10[96];
        PVOID                        PostProcessInitRoutine;
        BYTE                         Reserved11[128];
        PVOID                        Reserved12[1];
        DWORD                        SessionId;
    } PEB, *PPEB;

    typedef struct _TEB {
        PVOID Reserved1[12];
        PPEB  ProcessEnvironmentBlock;
        PVOID Reserved2[399];
        BYTE  Reserved3[1952];
        PVOID TlsSlots[64];
        BYTE  Reserved4[8];
        PVOID Reserved5[26];
        PVOID ReservedForOle;
        PVOID Reserved6[4];
        PVOID TlsExpansionSlots;
    } TEB, *PTEB;
}

[[nodiscard]]
inline nt::PE_HEADER* get_pe_header(void* module)
{
    nt::DOS_HEADER* dos_header{reinterpret_cast<nt::DOS_HEADER*>(module)};
    constexpr USHORT valid_dos_header{0x5A4D};

    return ((module) && dos_header->e_magic == valid_dos_header) ? CALC_ABSOLUTE(module, dos_header->e_lfanew, nt::PE_HEADER*) : nullptr;
}

[[nodiscard]]
inline nt::PIMAGE_EXPORT_DIRECTORY get_export_directory(void* module)
{
    nt::PE_HEADER* pe_header{get_pe_header(module)};

    return pe_header ? CALC_ABSOLUTE(module, pe_header->OptionalHeader.DataDirectory[0].VirtualAddress, nt::PIMAGE_EXPORT_DIRECTORY) : nullptr;
}

[[nodiscard]]
__forceinline
nt::PEB* get_peb()
{
#ifdef _WIN64
    return reinterpret_cast<nt::PEB*>(__readgsqword(FIELD_OFFSET(nt::TEB, ProcessEnvironmentBlock)));
#else
    return reinterpret_cast<nt::PEB*>(__readfsdword(FIELD_OFFSET(nt::TEB, ProcessEnvironmentBlock)));
#endif
}

[[nodiscard]]
__forceinline
nt::PLDR_DATA get_ldr_head()
{
    return CONTAINING_RECORD(get_peb()->Ldr->InMemoryOrderModuleList.Flink, nt::LDR_DATA, InMemoryOrderLinks);
}
