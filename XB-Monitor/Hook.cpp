// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#include "Hook.h"
#include "Global.h"

static int isInitialized = 0;

BOOL APIENTRY HookFunctions()
{
	// eX-BOARD software function hooks.
	HGetKeyLicense = (LPGetKeyLicense)HookIt("IpgExKey.dll", "_GetKeyLicense@0", Hook_GetKeyLicense);
	// Games run in a borderless 640x480 window. The wrapper forces the window to run in fullscreen.
	HOOK("D3D9.DLL", Direct3DCreate9, H, LP, Hook);
	// This avoids to get stuck in the 640x480 resolution when the process exits.
	HOOK("user32.dll", ChangeDisplaySettingsA, H, LP, Hook);
	// Prevents the process to send a shutdown request to the system and all running processes.
	HOOK("user32.dll", ExitWindowsEx, H, LP, Hook);
	
	// File operation function hooks.
	HOOK("kernel32.dll", CreateFileA, H, LP, Hook);
	HOOK("kernel32.dll", CreateFileW, H, LP, Hook);
	HOOK("kernel32.dll", ReadFile, H, LP, Hook);
	HOOK("kernel32.dll", WriteFile, H, LP, Hook);

	// Communications devices function hooks.
	HOOK("kernel32.dll", ClearCommError, H, LP, Hook);
	HOOK("kernel32.dll", CloseHandle, H, LP, Hook);
	HOOK("kernel32.dll", EscapeCommFunction, H, LP, Hook);
	HOOK("kernel32.dll", GetCommModemStatus, H, LP, Hook);
	HOOK("kernel32.dll", GetCommState, H, LP, Hook);
	HOOK("kernel32.dll", GetCommTimeouts, H, LP, Hook);
	HOOK("kernel32.dll", SetCommMask, H, LP, Hook);
	HOOK("kernel32.dll", SetCommState, H, LP, Hook);
	HOOK("kernel32.dll", SetCommTimeouts, H, LP, Hook);
	HOOK("kernel32.dll", SetupComm, H, LP, Hook);

	// DirectInput hooks.
	HOOK("DINPUT8.DLL", DirectInput8Create, F, LP, Fake);

	return TRUE;
}

INT APIENTRY Hook_GetKeyLicense(VOID)
{
	return 1;
}

LONG APIENTRY Hook_ChangeDisplaySettingsA(DEVMODE* lpDevMode, DWORD dwflags)
{
	return DISP_CHANGE_SUCCESSFUL;
}

BOOL APIENTRY Hook_ExitWindowsEx(UINT uFlags, DWORD dwReason)
{
	return FALSE;
}

static DWORD LastPoll = 0, PollsPerSec = 0;

BOOL APIENTRY Hook_ClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat)
{
	if (hFile != hConnection)
	{
		return HClearCommError(hFile, lpErrors, lpStat);
	}

	if (lpStat)
	{
		lpStat->cbInQue = !ReplyBuffer.empty() ? ReplyBuffer.size() : 0x00;

		if (IsAddressed())
		{
			DWORD tNow = GetTickCount();

			if ((tNow - LastPoll) >= 1000)
			{
				PollsPerSec = 0;
			}

			PollsPerSec++;
			LastPoll = tNow;

			lpStat->cbInQue += 8;
			ReplyBuffer.push(0x76);
			ReplyBuffer.push(0xFD);
			ReplyBuffer.push(0x08);
			ReplyBuffer.push(ControlByte0());
			ReplyBuffer.push(ControlByte1());
			ReplyBuffer.push(ControlByte2());
			ReplyBuffer.push(ControlByte3());
			ReplyBuffer.push(0x42);
		}
	}

	return TRUE;
}

BOOL APIENTRY Hook_CloseHandle(HANDLE hObject)
{
	if (hObject != hConnection)
	{
		return HCloseHandle(hObject);
	}

	else
	{
		ResetAddressed();
	}

	return TRUE;
}

BOOL APIENTRY Hook_EscapeCommFunction(HANDLE hFile, DWORD dwFunc)
{
	if (hFile != hConnection)
	{
		return HEscapeCommFunction(hFile, dwFunc);
	}

	return TRUE;
}

BOOL APIENTRY Hook_GetCommModemStatus(HANDLE hFile, LPDWORD lpModemStat)
{
	if (hFile != hConnection)
	{
		return HGetCommModemStatus(hFile, lpModemStat);
	}

	*lpModemStat = IsAddressed() ? 0x10 : 0;

	return TRUE;
}

BOOL APIENTRY Hook_GetCommState(HANDLE hFile, LPDCB lpDCB)
{
	if (hFile != hConnection)
	{
		return HGetCommState(hFile, lpDCB);
	}

	return TRUE;
}

BOOL APIENTRY Hook_GetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts)
{
	if (hFile != hConnection)
	{
		return HGetCommTimeouts(hFile, lpCommTimeouts);
	}

	return TRUE;
}

BOOL APIENTRY Hook_SetCommMask(HANDLE hFile, DWORD dwEvtMask)
{
	if (hFile != hConnection)
	{
		return HSetCommMask(hFile, dwEvtMask);
	}

	return TRUE;
}

BOOL APIENTRY Hook_SetCommState(HANDLE hFile, LPDCB lpDCB)
{
	if (hFile != hConnection)
	{
		return HSetCommState(hFile, lpDCB);
	}

	return TRUE;
}

BOOL APIENTRY Hook_SetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts)
{
	if (hFile != hConnection)
	{
		return HSetCommTimeouts(hFile, lpCommTimeouts);
	}

	return TRUE;
}

BOOL APIENTRY Hook_SetupComm(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue)
{
	if (hFile != hConnection)
	{
		return HSetupComm(hFile, dwInQueue, dwOutQueue);
	}

	return TRUE;
}

HANDLE APIENTRY Hook_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!strcmp(lpFileName, "COM1"))
	{
		if (!isInitialized)
		{
			InputMgr.Init();
			isInitialized = 1;
		}

		return hConnection;
	}

	return HCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE APIENTRY Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!wcscmp(lpFileName, L"COM1"))
	{
		if (!isInitialized)
		{
			InputMgr.Init();
			isInitialized = 1;
		}

		return hConnection;
	}
	
	return HCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL APIENTRY Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if (hFile != hConnection)
	{
		return HReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	}

	if (ReplyBuffer.size())
	{
		if (nNumberOfBytesToRead >= ReplyBuffer.size())
		{
			nNumberOfBytesToRead = ReplyBuffer.size();
		}

		*lpNumberOfBytesRead = nNumberOfBytesToRead;
		BYTE* pBuffer = (BYTE*)lpBuffer;

		for (DWORD i = 0; i < nNumberOfBytesToRead; i++)
		{
			if (!ReplyBuffer.empty())
			{
				*pBuffer++ = ReplyBuffer.front();
				ReplyBuffer.pop();
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
	}

	return TRUE;
}

BOOL APIENTRY Hook_WriteFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if (hFile != hConnection)
	{
		return HWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}

	BYTE* pBuffer = (BYTE*)lpBuffer;
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

	static BYTE rBuffer[1024];
	DWORD Stream = ProcessStream((LPBYTE)lpBuffer, nNumberOfBytesToWrite, rBuffer, 1024);

	if (Stream != 1)
	{
		for (DWORD i = 0; i < Stream; i++)
		{
			ReplyBuffer.push(rBuffer[i]);
		}
	}

	return TRUE;
}