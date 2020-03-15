//
// XB-Monitor: Opensource EX-Board loader by Romhack
//

#ifndef __TTX_SHARED_H
#define __TTX_SHARED_H

#include <iostream>
#include <dinput.h>

using namespace std;

#define MONITOR_INPUT_FILENAME	"monitor_input.bin"
#define MONITOR_MISC_FILENAME	"monitor_opt.bin"

#ifdef __TTX_CONFIG_BUILD__
#define logmsg printf
#else
#include "../XB-Monitor/XB-Monitor.h"
#endif


#define IS_JOY_OBJECT(n)	(n & 0x80000000)
#define IS_JOY_AXIS(n)		(n & 0x000F0000)
#define IS_JOY_POV(n)		(n & 0x000A0000)
#define GET_JOY_NUM(n)		((n >> 24) & 0x0F)
#define GET_JOY_BUT(n)		(n & 0xFF)
#define GET_JOY_AXIS(n)		((n >> 16) & 0xF)
#define GET_JOY_RANGE(n)	(n & 0xFFFF)
#define IS_NEGATIVE_AXIS(n)	((n >> 20) & 1)
#define AXIS_X				1
#define AXIS_Y				2
#define AXIS_Z				3
#define AXIS_RX				4
#define AXIS_RY				5
#define POVN				10

#define POV_CENTER			-1
#define POV_UP				0
#define POV_UP_RIGHT		4500
#define POV_RIGHT			9000
#define POV_RIGHT_DOWN		13500
#define POV_DOWN			18000
#define POV_DOWN_LEFT		22500
#define POV_LEFT			27000
#define POV_LEFT_UP			31500

#define MAX_AXIS_VAL		1000
#define DEADZONE_DIV		100
#define DEADZONE			500 //(MAX_AXIS_VAL / DEADZONE_DIV)
#define POLL_INTERVAL		30


extern BOOL bUniversalConfig;


typedef enum
{
	P1_START = 0,
	P1_COIN,
	P1_SERVICE,
	P1_UP,
	P1_DOWN,
	P1_LEFT,
	P1_RIGHT,
	P1_BUTTON_1,
	P1_BUTTON_2,
	P1_BUTTON_3,
	P1_BUTTON_4,
	P1_BUTTON_5,
	P1_BUTTON_6,
	P2_START,
	P2_COIN,
	P2_SERVICE,
	P2_UP,
	P2_DOWN,
	P2_LEFT,
	P2_RIGHT,
	P2_BUTTON_1,
	P2_BUTTON_2,
	P2_BUTTON_3,
	P2_BUTTON_4,
	P2_BUTTON_5,
	P2_BUTTON_6,
	TEST_MODE,
	TTX_EXIT_CODE,
	__INPUT_MAX__,
}
TTX_InputsDef;


typedef enum
{
	TTX_CONFIG_FULLSCREEN,
	TTX_CONFIG_POVASAXIS,
	__CONFIG_MAX__,
}
TTX_ConfigDef;


class TTX_ConfigManager
{
	public:
		
		DWORD optTable[__CONFIG_MAX__];
		
		TTX_ConfigManager();
		~TTX_ConfigManager();
		
		DWORD GetConfig(TTX_ConfigDef index);
		void SetConfig(TTX_ConfigDef index, DWORD val);
		void Config();
		void Load();
		void Save();
};

#endif