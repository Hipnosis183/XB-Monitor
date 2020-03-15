//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#include <dinput.h>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "Shared.h"
#include "DInput.h"

#include "../XB-Monitor/stdafx.h"

using namespace std;


TTX_InputManager::TTX_InputManager()
{
	lastPoll = 0;
	for (int i = P1_START; i < __INPUT_MAX__; i++)
		stateTable[i] = 0;
	di = NULL;
}


TTX_InputManager::~TTX_InputManager()
{
	#ifndef __TTX_CONFIG_BUILD__
	
	for (list<LPDIRECTINPUTDEVICE8>::iterator pdev = lpJoysticks.begin(); pdev != lpJoysticks.end(); pdev++)
		(*pdev)->Release();
	
	lpKeyboard->Release();

	#endif
}


int TTX_InputManager::Poll()
{
	#ifndef __TTX_CONFIG_BUILD__
	
	WaitForSingleObject(hPollMutex, INFINITE);

	static DIJOYSTATE2 js[16];

	for (list<LPDIRECTINPUTDEVICE8>::iterator pdev = lpJoysticks.begin(); pdev != lpJoysticks.end(); pdev++)
	{
		HRESULT res = (*pdev)->Poll();
		
		switch (res)
		{
			case DIERR_NOTACQUIRED:
			case DIERR_INPUTLOST:
			{
				res = (*pdev)->Acquire();
				break;
			}
		}
	}

	lpKeyboard->GetDeviceState(sizeof(keyBuffer), &keyBuffer);

	bool isPolled[16];
	
	for (int i = 0; i < 16; i++)
		isPolled[i] = false;

	for (int i = P1_START; i < __INPUT_MAX__; i++)
	{
		DWORD val = bindTable[i];

		if (IS_JOY_OBJECT(val))
		{
			DWORD joyNum = GET_JOY_NUM(val);
			LPDIRECTINPUTDEVICE8 lpDev = GetJoyDevice(joyNum);
			
			if (!lpDev) {
				stateTable[i] = 0;
				continue;
			}

			if (!isPolled[joyNum])
			{
				HRESULT res = lpDev->GetDeviceState(sizeof(DIJOYSTATE2), &js[joyNum]);
				
				switch (res)
				{
					case DIERR_NOTACQUIRED:
					case DIERR_INPUTLOST:
					{
						res = lpDev->Acquire();
						
						if (res != DI_OK)
							break;
					}
				}
				
				isPolled[joyNum] = true;
			}
			
			if (IS_JOY_AXIS(val))
			{
				WORD dir = js[joyNum].rgdwPOV[0];
				stateTable[i] = 0;
				
				switch (GET_JOY_AXIS(val))
				{
					case AXIS_X:
					{
						if (IS_NEGATIVE_AXIS(val))
						{
							if ((js[joyNum].lX < -DEADZONE) || ((configMgr.GetConfig(TTX_CONFIG_POVASAXIS)) && (dir == POV_LEFT || dir == POV_LEFT_UP || dir == POV_DOWN_LEFT)))
								stateTable[i] = 1;
						}

						else
						{
							if ((js[joyNum].lX > DEADZONE) || ((configMgr.GetConfig(TTX_CONFIG_POVASAXIS)) && (dir == POV_RIGHT || dir == POV_RIGHT_DOWN || dir == POV_UP_RIGHT)))
								stateTable[i] = 1;
						}
						
						break;
					}

					case AXIS_Y:
					{
						if (IS_NEGATIVE_AXIS(val))
						{
							if ((js[joyNum].lY < -DEADZONE) || ((configMgr.GetConfig(TTX_CONFIG_POVASAXIS)) && (dir == POV_UP || dir == POV_UP_RIGHT || dir == POV_LEFT_UP)))
								stateTable[i] = 1;
						}
							
						else
						{
							if ((js[joyNum].lY > DEADZONE) || ((configMgr.GetConfig(TTX_CONFIG_POVASAXIS)) && (dir == POV_DOWN || dir == POV_DOWN_LEFT || dir == POV_RIGHT_DOWN)))
								stateTable[i] = 1;
						}							

						break;
					}

					case AXIS_Z:
					{
						if (IS_NEGATIVE_AXIS(val))
						{
							if (js[joyNum].lZ < -DEADZONE)
								stateTable[i] = 1;
						}

						else
						{
							if (js[joyNum].lZ > DEADZONE)
								stateTable[i] = 1;
						}

						break;
					}

					case AXIS_RX:
					{
						if (IS_NEGATIVE_AXIS(val))
						{
							if (js[joyNum].lRx < -DEADZONE)
								stateTable[i] = 1;
						}

						else
						{
							if (js[joyNum].lRx > DEADZONE)
								stateTable[i] = 1;
						}

						break;
					}

					case AXIS_RY:
					{
						if (IS_NEGATIVE_AXIS(val))
						{
							if (js[joyNum].lRy < -DEADZONE)
								stateTable[i] = 1;
						}

						else
						{
							if (js[joyNum].lRy > DEADZONE)
								stateTable[i] = 1;
						}

						break;
					}
				
					case POVN:
					{
						// To avoid problems, mapped POVs are disabled and forced to work as Axis if PovAsAxis option is enabled
						
						if ((js[joyNum].rgdwPOV[0] != -1) && (configMgr.GetConfig(TTX_CONFIG_POVASAXIS) == 0))
						{
							if (js[joyNum].rgdwPOV[0] == GET_JOY_RANGE(val))
							{
								stateTable[i] = 1;
								break;
							}
						}
						
						break;
					}
				}
			}
			
			else
			{
				int but = GET_JOY_BUT(val);
				
				if (js[joyNum].rgbButtons[but] & 0x80)
					stateTable[i] = 1;
				else
					stateTable[i] = 0;
			}
		}

		else
		{
			if (keyBuffer[val & 0xFF] & 0x80)
				stateTable[i] = 1;
			else
				stateTable[i] = 0;
		}
	}
	
	ReleaseMutex(hPollMutex);
	
	#endif
	return 1;
}


BOOL TTX_InputManager::EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	TTX_InputManager *im = (TTX_InputManager *) pvRef;
	im->devices.push_back(lpddi);

	LPDIRECTINPUTDEVICE8 lpdev;
	
	if (im->di->CreateDevice(lpddi->guidInstance, &lpdev, NULL) == DI_OK)
	{
		lpdev->SetDataFormat(&c_dfDIJoystick2);
		im->lpJoysticks.push_back(lpdev);
		lpdev->EnumObjects((LPDIENUMDEVICEOBJECTSCALLBACK) &TTX_InputManager::EnumDeviceObjectsCallback, lpdev, DIDFT_ALL);

		return DIENUM_CONTINUE;
	}
	
	return DIENUM_STOP;
}


BOOL TTX_InputManager::EnumDeviceObjectsCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	LPDIRECTINPUTDEVICE8 dev = (LPDIRECTINPUTDEVICE8) pvRef;

	if (lpddoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		
		diprg.diph.dwSize = sizeof( DIPROPRANGE );
		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = lpddoi->dwType;
		diprg.lMin = -MAX_AXIS_VAL;
		diprg.lMax = +MAX_AXIS_VAL;
		
		dev->SetProperty(DIPROP_RANGE, &diprg.diph);

		DIPROPDWORD dipdw;
		
		memset(&dipdw, 0, sizeof( DIPROPDWORD ));
		dipdw.diph.dwSize = sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize =  sizeof(DIPROPHEADER);
		dipdw.diph.dwHow = DIPH_BYID;
		dipdw.diph.dwObj = lpddoi->dwType;
		dipdw.dwData = DEADZONE;

		dev->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	}

	return DIENUM_CONTINUE;
}


int TTX_InputManager::Load()
{
	FILE *fp = fopen(MONITOR_INPUT_FILENAME, "r");
	
	if (!fp)
		return 1;

	fread(bindTable, __INPUT_MAX__ * sizeof(DWORD), 1, fp);
	fclose(fp);
	
	return 0;
}


int TTX_InputManager::Save()
{
	FILE *fp = fopen(MONITOR_INPUT_FILENAME, "w");
	
	if (!fp)
		return 1;

	fwrite(bindTable, __INPUT_MAX__ * sizeof(DWORD), 1, fp);
	fclose(fp);
	
	return 0;
}


LPDIRECTINPUTDEVICE8 TTX_InputManager::GetJoyDevice(int n) {

	int i = 0;
	
	if (lpJoysticks.empty())
		return NULL;

	for (list<LPDIRECTINPUTDEVICE8>::iterator pdev = lpJoysticks.begin(); pdev != lpJoysticks.end(); pdev++)
	{
		if (i == n)
			return *pdev;
		else
			i++;
	}
	
	return NULL;
}


int TTX_InputManager::GetState(TTX_InputsDef inp)
{
	return stateTable[inp];
}


#ifndef __TTX_CONFIG_BUILD__

int DImagicCall = 0;
DWORD hPollThreadId = 0;


int TTX_InputManager::PollThread(TTX_InputManager *pThis)
{
	HANDLE currThread = GetCurrentThread();
	hPollThreadId = GetCurrentThreadId();
	SetThreadPriority(currThread, 1);

	pThis->Setup();
	pThis->Load();
	
	for (;;)
	{
		pThis->Poll();

		if (pThis->GetState(TTX_EXIT_CODE))
			ExitProcess(-1);
		
		Sleep(2);
	}
	
	return 0;
}
#endif


void TTX_InputManager::Setup()
{
	#ifndef __TTX_CONFIG_BUILD__
	
	hPollMutex = CreateMutex(0, 0, "poll_mutex");
	
	ZeroMemory(&wndcls, sizeof(WNDCLASS));
	
	wndcls.lpfnWndProc = DefWindowProcA;
	wndcls.hInstance = GetModuleHandle(0);
	wndcls.hbrBackground = (HBRUSH) 2;
	wndcls.lpszClassName = "dinput_poll_class";
	
	clsId = RegisterClass(&wndcls);
	
	if (!clsId)
	{
		DWORD err = GetLastError();
		
		if (err != ERROR_CLASS_ALREADY_EXISTS)
			ExitProcess(-1);
	}
	
	hSelfWin = CreateWindowEx(0, "dinput_poll_class", "dinput_poll", WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(0), NULL);
	
	if (!hSelfWin)
		ExitProcess(-2);

	DImagicCall = 1;
	
	if (DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *) &di, NULL) != DI_OK)
		ExitProcess(-3);
	
	DImagicCall = 0;

	di->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACKA) &TTX_InputManager::EnumDevicesCallback, (LPVOID) this,	0);

	for (list<LPDIRECTINPUTDEVICE8>::iterator i = lpJoysticks.begin(); i != lpJoysticks.end(); i++)
		(*i)->Acquire();

	di->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL);
	lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	lpKeyboard->SetCooperativeLevel(hSelfWin, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	lpKeyboard->Acquire();

	#endif
}


BOOL TTX_InputManager::Init()
{
	#ifndef __TTX_CONFIG_BUILD__
	
	DWORD id = 0;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE) &TTX_InputManager::PollThread, (LPVOID) this, 0, &id);
	
	#else
	Setup();
	#endif
	return TRUE;
}


void TTX_InputManager::Close()
{
	#ifndef __TTX_CONFIG_BUILD__
	#endif
}




