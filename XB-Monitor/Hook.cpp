//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#include <windows.h>
#include <string.h>
#include "stdafx.h"
#include <d3d9.h>
#include <list>

static const HANDLE hConnection = (HANDLE) 0xCAFEBEEF;

using namespace std;

std::queue<BYTE> replyBuffer;

static int isInitialized = 0;


static LPGetCommModemStatus __GetCommModemStatus = NULL;
static LPEscapeCommFunction __EscapeCommFunction = NULL;
static LPClearCommError __ClearCommError = NULL;
static LPWriteFile __WriteFile = NULL;
static LPReadFile __ReadFile = NULL;
static LPCloseHandle __CloseHandle = NULL;
static LPCreateFile  __CreateFile = NULL;
static LPSetupComm  __SetupComm = NULL;
static LPGetCommState  __GetCommState = NULL;
static LPSetCommState  __SetCommState = NULL;
static LPSetCommMask  __SetCommMask = NULL;
static LPSetCommTimeouts  __SetCommTimeouts = NULL;
static LPGetCommTimeouts  __GetCommTimeouts = NULL;
static LPGetWindowTextA __GetWindowTextA = NULL;
static LPGetWindowTextW __GetWindowTextW = NULL;
static LPGetAsyncKeyState __GetAsyncKeyState = NULL;
static LPCreateWindowExA __CreateWindowExA = NULL;
static LPCreateWindowExW __CreateWindowExW = NULL;
static LPCreateFileA  __CreateFileA = NULL;
static LPCreateFileW  __CreateFileW = NULL;
static LPGetKeyLicense __GetKeyLicense = NULL;
static LPChangeDisplaySettings __ChangeDisplaySettings = NULL;
static LPExitWindowsEx __ExitWindowsEx = NULL;
static LPShowCursor __ShowCursor = NULL;
static LPSetCursor __SetCursor = NULL;
static LPSetWindowPos __SetWindowPos = NULL;
static LPCreateEventA __CreateEventA = NULL;
static LPSleep __Sleep = NULL;


void patch_printf()
{
	DWORD oldProt = 0, newProt = PAGE_EXECUTE_READWRITE;
	
	VirtualProtect((void*)0x4386e6, 12, newProt, &oldProt);

	*(DWORD *)0x4386E6 = (DWORD) &NATIVE_logmsg;
	*(WORD * )0x4386E0 = 0x25ff;
	*(DWORD *)0x4386E2 = 0x4386E6;

	VirtualProtect((void*)0x4386e6, 12, oldProt, &newProt);
}


LPVOID __stdcall SetHookFunction(LPVOID dst, LPVOID lpHookFunction, LPCTSTR name)
{
	DWORD oldProt = 0, newProt = PAGE_EXECUTE_READWRITE;

	VirtualProtect(dst, 4, newProt, &oldProt);

	#if 1
	LPVOID ret = (LPVOID) *((DWORD*) dst); 
	#else
	LPVOID ret = (LPVOID) GetProcAddress(NULL, name);
	#endif

	*((DWORD *) dst) = (DWORD) lpHookFunction; 

	VirtualProtect(dst, 4, oldProt, &newProt);

	return ret;
}


#define __HOOK(addr, name) \
__ ##name = (LP ##name) SetHookFunction((LPVOID *) addr, &Hook_ ##name, #name)
#define __XHOOKA(mod, name) \
__ ##name = (LP ##name) HookIt(mod, #name "A", (LPVOID) &Hook_ ##name)
#define __XHOOKW(mod, name) \
__ ##name = (LP ##name) HookIt(mod, #name "W", (LPVOID) &Hook_ ##name)
#define __XHOOKn(mod, name) \
__ ##name = (LP ##name) HookIt(mod, #name, (LPVOID) &Hook_ ##name)
#define __XHOOKX(mod, ori, name) \
__ ##name = (LP ##name) HookIt(mod, #ori, (LPVOID) &Hook_ ##name)


BOOL __stdcall TTX_HookFunctions()
{
	/* EX-BOARD */
	
	__GetKeyLicense = (LPGetKeyLicense) HookIt("IpgExKey.dll", "_GetKeyLicense@0", Hook_GetKeyLicense);
	__XHOOKA("user32.dll", ChangeDisplaySettings);
	__XHOOKn("user32.dll", ExitWindowsEx);

	__XHOOKn("kernel32.dll", CreateFileA);
	__XHOOKn("kernel32.dll", CreateFileW);

	__XHOOKn("user32.dll", CreateWindowExA);
	__XHOOKn("user32.dll", SetWindowPos);

	#if 1
	
	__XHOOKn("kernel32.dll", WriteFile);
	__XHOOKn("kernel32.dll", ReadFile);
	__XHOOKn("kernel32.dll", CloseHandle);
	__XHOOKn("kernel32.dll", GetCommModemStatus);
	__XHOOKn("kernel32.dll", EscapeCommFunction);
	__XHOOKn("kernel32.dll", ClearCommError);
	__XHOOKn("kernel32.dll", SetCommMask);
	__XHOOKn("kernel32.dll", SetupComm);
	__XHOOKn("kernel32.dll", GetCommState);
	__XHOOKn("kernel32.dll", GetCommState);
	__XHOOKn("kernel32.dll", SetCommState);
	__XHOOKn("kernel32.dll", SetCommTimeouts);
	__XHOOKn("kernel32.dll", GetCommTimeouts);

	#endif
	
	__XHOOKn("dinput8.dll", DirectInput8Create);
	__XHOOKn("d3d9.dll", Direct3DCreate9);
	__XHOOKn("user32.dll", GetAsyncKeyState);
	
	return TRUE;
}


#undef __HOOK
#undef __XHOOKA
#undef __XHOOKn


void __stdcall Hook_Sleep(DWORD cnt)
{
}


HANDLE __stdcall Hook_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
	return __CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName);
}


int __stdcall Hook_GetKeyLicense(void)
{
	return 1;
}


BOOL __stdcall Hook_SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	return TRUE;
}


HWND __stdcall Hook_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (configMgr.GetConfig(TTX_CONFIG_FULLSCREEN))
	{
		dwExStyle = 0;
		dwStyle = WS_EX_TOPMOST | WS_POPUP;
		
		x = 0;
		y = 0;

		return __CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	} 

	dwExStyle = 0;
	dwStyle = WS_OVERLAPPEDWINDOW;

	RECT r;
	r.right = nWidth;
	r.bottom = nHeight;
	r.top = 0;
	r.left = 0;
	
	AdjustWindowRect(&r, dwStyle, FALSE);

	nWidth = r.right-r.left;
	nHeight = r.bottom - r.top;

	return __CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, 0, 0, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}


HWND __stdcall Hook_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (configMgr.GetConfig(TTX_CONFIG_FULLSCREEN))
	{
		dwExStyle = 0;
		dwStyle = WS_EX_TOPMOST | WS_POPUP;
		
		x = 0;
		y = 0;

		return __CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}
	
	dwExStyle =0;
	dwStyle = WS_OVERLAPPEDWINDOW;
	
	RECT r;
	r.bottom = nHeight;
	r.top = 0;
	r.right = nWidth;
	r.left = 0;
	
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	return __CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, r.right, r.bottom, hWndParent, hMenu, hInstance, lpParam);
}


LONG __stdcall Hook_ChangeDisplaySettings(DEVMODE *lpDevMode, DWORD dwflags)
{
	return DISP_CHANGE_SUCCESSFUL;
}


BOOL __stdcall Hook_ExitWindowsEx(UINT uFlags, DWORD dwReason)
{
	return FALSE;
}


int __stdcall Hook_ShowCursor(BOOL bShow)
{
	return __ShowCursor(TRUE);
}


HCURSOR __stdcall Hook_SetCursor(HCURSOR hCursor)
{
	return NULL;
}


SHORT __stdcall Hook_GetAsyncKeyState(int vKey)
{
	return 0;
}


BOOL __stdcall Hook_GetCommModemStatus(HANDLE hFile, LPDWORD lpModemStat)
{
	if (hFile != hConnection)
		return __GetCommModemStatus(hFile, lpModemStat);

	if (is_addressed())
		*lpModemStat = 0x10;
	else
		*lpModemStat = 0;

	return TRUE;
}


BOOL __stdcall Hook_EscapeCommFunction(HANDLE hFile, DWORD dwFunc)
{
	if (hFile != hConnection)
		return __EscapeCommFunction(hFile, dwFunc);

	return TRUE;
}


unsigned char control_byte_0()
{
	unsigned char s = 0;

	if (inputMgr.GetState(P1_START))
		s |= 0x80;
	if (inputMgr.GetState(P1_SERVICE) || inputMgr.GetState(P2_SERVICE))
		s |= 0x40;
	if (inputMgr.GetState(P1_UP))
		s |= 0x20;
	if (inputMgr.GetState(P1_DOWN))
		s |= 0x10;
	if (inputMgr.GetState(P1_RIGHT))
		s |= 0x04;
	if (inputMgr.GetState(P1_LEFT))
		s |= 0x08;
	if (inputMgr.GetState(P1_BUTTON_1))
		s |= 0x02;
	if (inputMgr.GetState(P1_BUTTON_2))
		s |= 0x01;
	
	return s;
}


unsigned char control_byte_1()
{
	unsigned char s = 0;
	
	if (inputMgr.GetState(P1_COIN))
		s |= 0x01;
	if (inputMgr.GetState(P1_BUTTON_3))
		s |= 0x80;
	if (inputMgr.GetState(P1_BUTTON_4))
		s |= 0x40;
	if (inputMgr.GetState(P1_BUTTON_5))
		s |= 0x20;
	if (inputMgr.GetState(P1_BUTTON_6))
		s |= 0x10;
	
	return s;
}


unsigned char control_byte_2()
{
	unsigned char s = 0;
	
	if (inputMgr.GetState(P2_START))
		s |= 0x80;
	if (inputMgr.GetState(TEST_MODE))
		s |= 0x40;
	if (inputMgr.GetState(P2_UP))
		s |= 0x20;
	if (inputMgr.GetState(P2_DOWN))
		s |= 0x10;
	if (inputMgr.GetState(P2_RIGHT))
		s |= 0x04;
	if (inputMgr.GetState(P2_LEFT))
		s |= 0x08;
	if (inputMgr.GetState(P2_BUTTON_1))
		s |= 0x02;
	if (inputMgr.GetState(P2_BUTTON_2))
		s |= 0x01;
	
	return s;
}


unsigned char control_byte_3()
{
	unsigned char s = 0;
	
	if (inputMgr.GetState(P2_COIN))
		s |= 0x01;
	if (inputMgr.GetState(P2_BUTTON_3))
		s |= 0x80;
	if (inputMgr.GetState(P2_BUTTON_4))
		s |= 0x40;
	if (inputMgr.GetState(P2_BUTTON_5))
		s |= 0x20;
	if (inputMgr.GetState(P2_BUTTON_6))
		s |= 0x10;
	
	return s;
}


static DWORD last_i_poll = 0, polls_per_sec = 0;


BOOL __stdcall Hook_ClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat)
{
	if (hFile != hConnection)
		return __ClearCommError(hFile, lpErrors, lpStat);

	if (lpStat)
	{
		if (!replyBuffer.empty())
			lpStat->cbInQue = replyBuffer.size();
		else
			lpStat->cbInQue = 0;
		
		if (is_addressed())
		{
			DWORD now = GetTickCount();
			
			if ((now - last_i_poll) >= 1000)
				polls_per_sec = 0;

			polls_per_sec++;
			last_i_poll = now;
			
			lpStat->cbInQue += 8;
			replyBuffer.push(0x76);
			replyBuffer.push(0xfd);
			replyBuffer.push(0x08);
			replyBuffer.push(control_byte_0());
			replyBuffer.push(control_byte_1());
			replyBuffer.push(control_byte_2());
			replyBuffer.push(control_byte_3());
			replyBuffer.push(0x42);
		}
	}

	return TRUE;
}


BOOL __stdcall Hook_SetupComm(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue)
{
	if (hFile != hConnection)
		return __SetupComm(hFile, dwInQueue, dwOutQueue);

	return TRUE;
}


BOOL __stdcall Hook_GetCommState(HANDLE hFile, LPDCB lpDCB)
{
	if (hFile != hConnection)
		return __GetCommState(hFile, lpDCB);

	return TRUE;
}


BOOL __stdcall Hook_SetCommState(HANDLE hFile, LPDCB lpDCB)
{
	if (hFile != hConnection)
		return __SetCommState(hFile, lpDCB);
	
	return TRUE;
}


BOOL __stdcall Hook_SetCommMask(HANDLE hFile, DWORD dwEvtMask)
{
	if (hFile != hConnection)
		return __SetCommMask(hFile, dwEvtMask);

	return TRUE;
}


BOOL __stdcall Hook_GetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts)
{
	if (hFile != hConnection)
		return __GetCommTimeouts(hFile, lpCommTimeouts);

	return TRUE;
}


BOOL __stdcall Hook_SetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts)
{
	if (hFile != hConnection)
		return __SetCommTimeouts(hFile, lpCommTimeouts);

	return TRUE;
}


BOOL __stdcall Hook_WriteFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if (hFile != hConnection)
		return __WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

	BYTE *ptr = (BYTE *) lpBuffer;

	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	
	static BYTE rbuffer[1024];
	DWORD sz = process_stream((LPBYTE)lpBuffer, nNumberOfBytesToWrite, rbuffer, 1024);
	
	if (sz != 1)
	{
		for (DWORD i=0; i < sz; i++)
			replyBuffer.push(rbuffer[i]);
	}

	return TRUE;
}


BOOL __stdcall Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if (hFile != hConnection)
		return __ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

	if (replyBuffer.size())
	{
		if (nNumberOfBytesToRead >= replyBuffer.size())
			nNumberOfBytesToRead = replyBuffer.size();
		
		*lpNumberOfBytesRead = nNumberOfBytesToRead;
		BYTE *ptr = (BYTE*) lpBuffer;
		
		for (DWORD i=0; i < nNumberOfBytesToRead; i++)
		{
			if (!replyBuffer.empty())
			{
				*ptr++ = replyBuffer.front();
				replyBuffer.pop();
			}
			
			else
			{
				*lpNumberOfBytesRead = i;
				break;
			}
		}
	}
	
	else
	{
		*lpNumberOfBytesRead = 0;
		return TRUE;
	}

	return TRUE;
}


BOOL __stdcall Hook_CloseHandle(HANDLE hObject)
{
	if (hObject != hConnection)
		return __CloseHandle(hObject);
	else
		reset_addressed();

	return TRUE;
}


HANDLE __stdcall Hook_CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!strcmp(lpFileName, "COM1"))
	{
		if (!isInitialized)
		{
			inputMgr.Init();
			isInitialized = 1;
		}
		
		return hConnection;

	}
	
	else
		return __CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


int _mbsnbcmp(const char *a, const char *b, int l)
{
	int ret = 0;
	
	for (int i=0;i<l;i++)
	{
		ret += ((int)*a - (int)*b);
		++a;
		++b;
	}
	
	return ret;
}

HANDLE __stdcall Hook_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!strcmp(lpFileName, "COM1"))
	{
		if (!isInitialized)
		{
			inputMgr.Init();
			isInitialized = 1;
		}
		
		return hConnection;
	}
	
	return __CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE __stdcall Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!wcscmp(lpFileName, L"COM1"))
	{
		if (!isInitialized)
		{
			inputMgr.Init();
			isInitialized = 1;
		}
		return hConnection;

	}
	
	else
		return __CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}