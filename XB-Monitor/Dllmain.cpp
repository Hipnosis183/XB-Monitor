// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#include "Global.h"

HANDLE hMutex;
HINSTANCE hInstance;

InputManager InputMgr;
ConfigManager ConfigMgr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			hMutex = CreateMutex(0, 0, NULL);
			hInstance = GetModuleHandle(0);

			// Uncomment to use logging functions.
			// LogInitialize();

			// Initialize the configuration.
			ConfigMgr.Load();
			ConfigMgr.LoadTable();

			// Initialize SRAM.
			CreateDirectory("sv", 0);
			LoadSRAM();

			// Prepare the hooking function.
			HookItInit();
			// Start the WinAPI and DInput hooking.
			HookFunctions();

			break;
		}
		
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH: break;

		case DLL_PROCESS_DETACH:
		{
			SaveSRAM();

			break;
		}
	}

	return TRUE;
}