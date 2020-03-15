//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#include "stdafx.h"
#include "udis86/udis86.h"

static LPBYTE hookAddrBase = NULL;
static LPBYTE hookAddr = NULL;
static LPBYTE dasmAddr = NULL;
static ud_t ud_obj;

static int DasmInputHookEIP(ud_t* u)
{
	int val = (int) *dasmAddr;
	++dasmAddr;
	
	return val;
}

DWORD DasmOp(LPVOID addr)
{
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_input_hook(&ud_obj, DasmInputHookEIP);
	
	dasmAddr = (LPBYTE) addr;
	
	return (DWORD) ud_disassemble(&ud_obj);
}

void __stdcall TTX_HookItInit()
{
	hookAddrBase = hookAddr = (LPBYTE) VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

LPVOID __stdcall HookIt(LPCTSTR module, LPCTSTR name, LPVOID lpHookFunction)
{
	BYTE oriBytes[32], restoreSize = 0;
	LPVOID _apiAddr = (LPVOID) GetProcAddress(GetModuleHandle(module), name);
	
	if (!_apiAddr)
		return NULL;
	
	LPBYTE _addr = (LPBYTE) _apiAddr;
	DWORD oldProt = 0, newProt = 0;

	while (restoreSize < 6)
	{
		DWORD opsize = DasmOp(_addr);
		restoreSize += opsize;
		_addr += opsize;
	}
	
	VirtualProtect(_apiAddr, restoreSize, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy(oriBytes, _apiAddr, restoreSize);

	LPBYTE tempAddr = (LPBYTE)_apiAddr;
	*tempAddr = 0xE9;
	++tempAddr;
	*((DWORD *)tempAddr) = ((signed int)(DWORD)(lpHookFunction)) - ((signed int)((DWORD)tempAddr+4));

	LPBYTE oriAddrCall = hookAddr;
	memcpy(hookAddr, oriBytes, restoreSize);
	hookAddr += restoreSize;

	tempAddr = hookAddr;
	*tempAddr = 0xE9;
	++tempAddr;
	*((DWORD *)tempAddr) = ((signed int)((DWORD)_apiAddr)+restoreSize) - ((signed int)((DWORD)tempAddr+4));
	hookAddr += 5;
	
	return oriAddrCall;
}