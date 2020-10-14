// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#pragma once
#include <d3d9.h>
#include <dinput.h>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <thread>
#include <Windows.h>

#include "SetInput.h"
#include "SetOptions.h"

#define _WIN32_WINDOWS 0x0410
#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x800

typedef unsigned char UINT8;
extern DWORD mTable[CONFIG_END];

extern HANDLE hMutex;
extern HINSTANCE hInstance;

extern InputManager InputMgr;
extern ConfigManager ConfigMgr;

int IsAddressed();
void ResetAddressed();
DWORD ProcessStream(UINT8 *stream, DWORD srcsize, BYTE *dst, DWORD dstsize);

// DirectInput
extern int DIMagicCall;

HRESULT APIENTRY Fake_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
typedef HRESULT(APIENTRY* LPDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
extern LPDirectInput8Create FDirectInput8Create;

// Direct3D
IDirect3D9* APIENTRY Hook_Direct3DCreate9(UINT SDKVersion);
typedef IDirect3D9* (APIENTRY* LPDirect3DCreate9)(UINT SDKVersion);
extern LPDirect3DCreate9 HDirect3DCreate9;

// SRAM.
#define SRAM_SIZE	0xffff
#define SRAM_NAME	"sv/sram.bin"

VOID SaveSRAM();
VOID LoadSRAM();

extern UINT8 SRAM[SRAM_SIZE];
extern UINT8 ControlByte0();
extern UINT8 ControlByte1();
extern UINT8 ControlByte2();
extern UINT8 ControlByte3();


// Hooking engine.
VOID APIENTRY HookItInit();
BOOL APIENTRY HookFunctions();
LPVOID APIENTRY HookIt(LPCTSTR Module, LPCTSTR Name, LPVOID lpHookFunction);

// Logging engine.
void APIENTRY LogInitialize();
void APIENTRY LogMessage(const CHAR* TextMessage, ...);
void APIENTRY LogMessageW(const WCHAR* TextMessage, ...);