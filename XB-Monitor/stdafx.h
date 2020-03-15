//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#pragma once

#include <queue>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <dinput.h>
#include <d3d9.h>
#include "targetver.h"
#include "XB-Monitor.h"

#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x800

#include "../Shared/DInput.h"

extern HINSTANCE hInstance;
extern TTX_InputManager inputMgr;
extern TTX_ConfigManager configMgr;
extern HANDLE hMutex;

DWORD process_stream(UINT8 *stream, DWORD srcsize, BYTE *dst, DWORD dstsize);

int is_addressed();
void reset_addressed();

IDirect3D9* __stdcall Hook_Direct3DCreate9(UINT SDKVersion);
typedef IDirect3D9* (__stdcall* LPDirect3DCreate9)(UINT SDKVersion);

extern LPDirect3DCreate9 __Direct3DCreate9;

HRESULT __stdcall Hook_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);

typedef HRESULT (__stdcall *LPDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);

extern LPDirectInput8Create __DirectInput8Create;
extern int DImagicCall;

typedef HANDLE (__stdcall *LPCreateEventA)(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
typedef BOOL (__stdcall *LPSetWindowPos)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
typedef HCURSOR (__stdcall *LPSetCursor)(HCURSOR hCursor);
typedef int (__stdcall *LPShowCursor)(BOOL bShow);
typedef int (__stdcall *LPSleep)(DWORD);
typedef BOOL (__stdcall *LPExitWindowsEx)(UINT uFlags, DWORD dwReason);
typedef LONG (__stdcall *LPChangeDisplaySettings)(DEVMODE *lpDevMode, DWORD dwflags);
typedef int (__stdcall *LPGetWindowTextA)(HWND hWnd, LPSTR lpString, int nMaxCount);
typedef int (__stdcall *LPGetWindowTextW)(HWND hWnd, LPWSTR lpString, int nMaxCount);
typedef int (__stdcall *LPGetKeyLicense)(void);
typedef BOOL (__stdcall *LPGetCommModemStatus)(HANDLE hFile, LPDWORD lpModemStat);
typedef BOOL (__stdcall *LPEscapeCommFunction)(HANDLE hFile, DWORD dwFunc);
typedef BOOL (__stdcall *LPClearCommError)(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat);
typedef BOOL (__stdcall *LPSetupComm)(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue);
typedef BOOL (__stdcall *LPGetCommState)(HANDLE hFile, LPDCB lpDCB);
typedef BOOL (__stdcall *LPSetCommState)(HANDLE hFile, LPDCB lpDCB);
typedef BOOL (__stdcall *LPSetCommMask)(HANDLE hFile, DWORD dwEvtMask);
typedef BOOL (__stdcall *LPGetCommTimeouts)(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts);
typedef BOOL (__stdcall *LPSetCommTimeouts)(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts);
typedef BOOL (__stdcall *LPWriteFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
typedef BOOL (__stdcall *LPReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
typedef BOOL (__stdcall *LPCloseHandle)(HANDLE hObject);
typedef HANDLE (__stdcall *LPCreateFile)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE (__stdcall *LPCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE (__stdcall *LPCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HWND (__stdcall *LPCreateWindowExW)(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef HWND (__stdcall *LPCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef SHORT (__stdcall *LPGetAsyncKeyState)(int vKey);

HANDLE __stdcall Hook_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
BOOL __stdcall Hook_SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
int __stdcall Hook_ShowCursor(BOOL bShow);
void __stdcall Hook_Sleep(DWORD cnt);
HCURSOR __stdcall Hook_SetCursor(HCURSOR hCursor);
BOOL __stdcall Hook_ExitWindowsEx(UINT uFlags, DWORD dwReason);
LONG __stdcall Hook_ChangeDisplaySettings(DEVMODE *lpDevMode, DWORD dwflags);
int __stdcall Hook_GetKeyLicense(void);
SHORT __stdcall Hook_GetAsyncKeyState(int vKey);
BOOL __stdcall Hook_GetCommModemStatus(HANDLE hFile, LPDWORD lpModemStat);
BOOL __stdcall Hook_EscapeCommFunction(HANDLE hFile, DWORD dwFunc);
BOOL __stdcall Hook_ClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat);
BOOL __stdcall Hook_SetupComm(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue);
BOOL __stdcall Hook_GetCommState(HANDLE hFile, LPDCB lpDCB);
BOOL __stdcall Hook_SetCommState(HANDLE hFile, LPDCB lpDCB);
BOOL __stdcall Hook_SetCommMask(HANDLE hFile, DWORD dwEvtMask);
BOOL __stdcall Hook_GetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts);
BOOL __stdcall Hook_SetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts);
BOOL __stdcall Hook_WriteFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
BOOL __stdcall Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL __stdcall Hook_CloseHandle(HANDLE hObject);
HANDLE __stdcall Hook_CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE __stdcall Hook_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE __stdcall Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HWND __stdcall Hook_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND __stdcall Hook_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
