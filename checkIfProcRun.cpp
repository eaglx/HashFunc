#include <Windows.h>
#include <iostream>
#include "apifunhashed.hpp"

typedef HANDLE (WINAPI* FNPTR)(DWORD, DWORD);

int checkIfProcRun() {
	PROCESSENTRY32 pe32;
	FNPTR funcName = (FNPTR)getApiAddr(GetModuleHandleA("kernel32.dll"), hashCreateToolhelp32Snapshot);
	HANDLE hProcessSnap = func(TH32CS_SNAPPROCESS, 0); // Take a snapshot of all processes in the system
	
	if (hProcessSnap == INVALID_HANDLE_VALUE) return -1;
	else {
		pe32.dwSize = sizeof(PROCESSENTRY32); // Set the size of the structure before using it.
		if (Process32First(hProcessSnap, &pe32)) { // Gets first running process
			// loop through all running processes looking for process
			while (Process32Next(hProcessSnap, &pe32)) {
				// Set to an AnsiString instead of Char[] to make compare easier
				std::cout << pe32.szExeFile << "\n";
			}
			// clean the snapshot object
			CloseHandle(hProcessSnap);
		}
	}
	return 0;
}