//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#include <direct.h>
#include "stdafx.h"

static bool is_attached = false;

HINSTANCE hInstance;

TTX_InputManager inputMgr;
TTX_ConfigManager configMgr;

HANDLE hMutex;


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			hMutex = CreateMutex(0, 0, NULL);

			// loginit(); // Uncomment to use logging functions

			hInstance = GetModuleHandle(0);
			configMgr.Load();

			CreateDirectory("sv", 0);

			SRAM_load();

			TTX_HookItInit();
			TTX_HookFunctions();

			break;
		}			
		
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		
		case DLL_PROCESS_DETACH:
		{
			SRAM_save();
			break;
		}			
	}

	return TRUE;
}



