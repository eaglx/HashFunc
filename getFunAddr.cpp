#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <libloaderapi.h>
#include "apifunhashed.hpp"

#define RVATOVA( base, offset ) ( (SIZE_T)base + (SIZE_T)offset )
typedef unsigned int uint;

#include <typeinfo>

void* getApiAddr(HMODULE module, long hashFunc) {
	if (module == nullptr) return nullptr;

	PIMAGE_OPTIONAL_HEADER poh = (PIMAGE_OPTIONAL_HEADER)((LPVOID)((SIZE_T)module + ((PIMAGE_DOS_HEADER)(module))->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER)));
	PIMAGE_EXPORT_DIRECTORY exportDir = (IMAGE_EXPORT_DIRECTORY*)RVATOVA(module, poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	int exportSize = poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	int ordinal = -1;

	DWORD * namesTable = (DWORD*)RVATOVA(module, exportDir->AddressOfNames);
	WORD * ordinalTable = (WORD*)RVATOVA(module, exportDir->AddressOfNameOrdinals);

	for (uint i = 0; i < exportDir->NumberOfNames; i++)
	{
		char* name = (char*)RVATOVA(module, *namesTable);
		long intValue = pjwHash(name);

		if (intValue == hashFunc)
		{
			ordinal = *ordinalTable;
			break;
		}

		namesTable++;
		ordinalTable++;
	}

	if (ordinal < 0) return nullptr;
	DWORD* addrTable = (DWORD*)RVATOVA(module, exportDir->AddressOfFunctions);
	SIZE_T rva = addrTable[ordinal];
	SIZE_T addr = (SIZE_T)RVATOVA(module, rva);
	return (void*)addr;
}