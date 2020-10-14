// Fake DirectInput by Romhack.

#include "IDInDevFake.h"
#include "Global.h"

#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800

LPDirectInput8Create FDirectInput8Create;

// Prevents the games of having access to input devices.
HRESULT APIENTRY Fake_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	if (DIMagicCall)
	{
		return FDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	}

	else
	{
		*ppvOut = (LPVOID)pFakeInterface;
		punkOuter = NULL;

		return DI_OK;
	}
}