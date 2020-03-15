//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#ifndef __TTX_INPUT_H
#define __TTX_INPUT_H
#define DIRECTINPUT_VERSION 0x800

#include <dinput.h>
#include <iostream>
#include <string>
#include <list>

#include "Shared.h"

using namespace std;

#ifndef __TTX_CONFIG_BUILD__

extern DWORD hPollThreadId;
extern int DImagicCall;

#endif


class TTX_InputManager
{
	LPDIRECTINPUT8 di;

	static BOOL EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static BOOL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	char keyBuffer[256];
	int stateTable[__INPUT_MAX__];	
	DWORD bindTable[__INPUT_MAX__];
	DWORD lastPoll;

	list<LPCDIDEVICEINSTANCE> devices;
	list<LPDIRECTINPUTDEVICE8> lpJoysticks;
	LPDIRECTINPUTDEVICE8 lpKeyboard;
	LPDIRECTINPUTDEVICE8 GetJoyDevice(int n);

	void Setup();
	HWND hSelfWin;

	#ifndef __TTX_CONFIG_BUILD__
	
	WNDCLASS wndcls;
	HANDLE hPollMutex;
	ATOM clsId;

	static int PollThread(TTX_InputManager *pThis);
	DWORD threadId;

	#endif
	
	public:
	
		char strBuf[128];

		TTX_InputManager();
		~TTX_InputManager();

		void Close();
		int Poll();
		int GetState(TTX_InputsDef inp);
		int Save();
		int Load();
		BOOL Init();
};

#endif