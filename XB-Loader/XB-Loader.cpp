//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#include <windows.h>
#include <iostream>
#include <string>

#include "stdafx.h"

using namespace std;


class LProcess
{
	PROCESS_INFORMATION pi;
	BOOL is_loaded;
	STARTUPINFO si;
	void Error(TCHAR *msg);
	HANDLE dllHandle;

	public:
		
		void Wait();
		DWORD Resume();

		LProcess(const TCHAR* name);
		~LProcess();

		DWORD Write(void *buf, void *dst, int len);
		DWORD Read(void *buf, void *src, int len);
		
		void Patch(DWORD addr, BYTE data)
		{
			if (is_loaded)
				Write(&data, (void *) (addr + 0x400000), 1);
		}
		
		BOOL InjectDLL(const TCHAR *name);
};


void LProcess::Error(TCHAR *msg)
{
	MessageBox(0, msg, _TEXT("Error"), MB_OK);
	exit(1);
}


LProcess::LProcess(const TCHAR *name): is_loaded(FALSE)
{
	ZeroMemory(&si, sizeof(si));
	
	si.cb = sizeof(si);

	is_loaded = CreateProcess(name, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	is_loaded = TRUE;
}


DWORD LProcess::Write(void *buf, void *dst, int len)
{
	DWORD ret = 0;
	DWORD oldProt, newProt;

	WriteProcessMemory(pi.hProcess, dst, buf, len, &ret);

	return ret;
}


DWORD LProcess::Read(void *buf, void *src, int len)
{
	DWORD ret = 0;
	DWORD oldProt, newProt;

	ReadProcessMemory(pi.hProcess, src, buf, len, &ret);

	return ret;
}


DWORD LProcess::Resume()
{
	return ResumeThread(pi.hThread);
}


void LProcess::Wait()
{
	if (is_loaded)
		WaitForSingleObject(pi.hProcess, INFINITE);
}


BOOL LProcess::InjectDLL(const TCHAR *name)
{
	LPVOID _lproc;
	LPVOID _pbase;
	DWORD threadid;
	TCHAR errbuf[256];

	_lproc = (LPVOID) GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
	
	if (!_lproc)
		Error(errbuf);

	_pbase = VirtualAllocEx(pi.hProcess, NULL, _tcslen(name), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	
	if (!_pbase)
		Error(errbuf);
	
	Write((void *)name, _pbase, _tcslen(name));

	dllHandle = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE) _lproc, _pbase, 0, &threadid);
	
	if (!dllHandle)
		Error(errbuf);

	WaitForSingleObject(dllHandle, INFINITE);
	
	return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	
	LProcess *proc = NULL;
	
	proc = new LProcess(argv[1]);
	
	proc->InjectDLL(_T("xb_monitor.dll"));
	proc->Resume();
	proc->Wait();
	
	return 0;
}

