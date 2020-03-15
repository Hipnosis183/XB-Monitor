//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TTX_MONITOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TTX_MONITOR_API  functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef TTX_MONITOR_EXPORTS
#define TTX_MONITOR_API  __declspec(dllexport)
#else
#define TTX_MONITOR_API  __declspec(dllimport)
#endif

#include <windows.h>

#define SRAM_SIZE	0xffff
#define SRAM_NAME	"sv/sram.bin"

void __cdecl NATIVE_logmsg(const char *fmt, ...);
void __stdcall logmsg(const char *fmt, ...);
void __stdcall logmsgw(const WCHAR *fmt, ...);
void __stdcall logmsg_nt(const char *fmt, ...);
void __stdcall loginit();
BOOL __stdcall TTX_HookFunctions();

extern unsigned char SRAM[SRAM_SIZE];
void SRAM_save();
void SRAM_load();

DWORD TTX_MONITOR_API TTXConfig();

void __stdcall TTX_HookItInit();
LPVOID __stdcall HookIt(LPCTSTR module, LPCTSTR name, LPVOID lpHookFunction);
