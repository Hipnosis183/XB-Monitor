// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#pragma once
#include "Global.h"

UINT8 SRAM[SRAM_SIZE];

//  Not much documentation over here, nor somewhere else.
//
//  Reply format:
//
// +------+---------+------+-----+-------+------+
// | 0x76 | COMMAND | SIZE | ??? | REPLY | 0x42 |
// +------+---------+------+-----+-------+------+
//
//  Switch input data:
//
// +--------+--------+---------+--------+--------+-------+-------+--------+--------+
// |        | BIT 7  | BIT 6   | BIT 5  | BIT 4  | BIT 3 | BIT 2 | BIT 1  | BIT 0  |
// +--------+--------+---------+--------+--------+-------+-------+--------+--------+
// | Byte 0 | START  |  TEST   | UP     | DOWN   | LEFT  | RIGHT | PUSH 1 | PUSH 2 |
// +--------+--------+---------+--------+--------+-------+-------+--------+--------+
// | Byte 1 | PUSH 3 | PUSH 4  | PUSH 5 | PUSH 6 |       |       |        |  COIN  |
// +--------+--------+---------+--------+--------+-------+-------+--------+--------+
//
// Buttons 5 and 6 are JAMMA compatible and added externally to the JVS.

UINT8 ControlByte0()
{
	UINT8 Byte = 0;

	if (InputMgr.GetState(P1_START))
	{
		Byte |= 0x80;
	}

	if (InputMgr.GetState(P1_SERVICE) || InputMgr.GetState(P2_SERVICE))
	{
		Byte |= 0x40;
	}

	if (InputMgr.GetState(P1_UP))
	{
		Byte |= 0x20;
	}

	if (InputMgr.GetState(P1_DOWN))
	{
		Byte |= 0x10;
	}

	if (InputMgr.GetState(P1_RIGHT))
	{
		Byte |= 0x04;
	}

	if (InputMgr.GetState(P1_LEFT))
	{
		Byte |= 0x08;
	}

	if (InputMgr.GetState(P1_BUTTON_1))
	{
		Byte |= 0x02;
	}

	if (InputMgr.GetState(P1_BUTTON_2))
	{
		Byte |= 0x01;
	}

	return Byte;
}

UINT8 ControlByte1()
{
	UINT8 Byte = 0;

	if (InputMgr.GetState(P1_COIN))
	{
		Byte |= 0x01;
	}

	if (InputMgr.GetState(P1_BUTTON_3))
	{
		Byte |= 0x80;
	}

	if (InputMgr.GetState(P1_BUTTON_4))
	{
		Byte |= 0x40;
	}

	if (InputMgr.GetState(P1_BUTTON_5))
	{
		Byte |= 0x20;
	}

	if (InputMgr.GetState(P1_BUTTON_6))
	{
		Byte |= 0x10;
	}

	return Byte;
}

UINT8 ControlByte2()
{
	UINT8 Byte = 0;

	if (InputMgr.GetState(P2_START))
	{
		Byte |= 0x80;
	}

	if (InputMgr.GetState(TEST_MODE))
	{
		Byte |= 0x40;
	}

	if (InputMgr.GetState(P2_UP))
	{
		Byte |= 0x20;
	}

	if (InputMgr.GetState(P2_DOWN))
	{
		Byte |= 0x10;
	}

	if (InputMgr.GetState(P2_RIGHT))
	{
		Byte |= 0x04;
	}

	if (InputMgr.GetState(P2_LEFT))
	{
		Byte |= 0x08;
	}

	if (InputMgr.GetState(P2_BUTTON_1))
	{
		Byte |= 0x02;
	}

	if (InputMgr.GetState(P2_BUTTON_2))
	{
		Byte |= 0x01;
	}

	return Byte;
}

UINT8 ControlByte3()
{
	UINT8 Byte = 0;

	if (InputMgr.GetState(P2_COIN))
	{
		Byte |= 0x01;
	}

	if (InputMgr.GetState(P2_BUTTON_3))
	{
		Byte |= 0x80;
	}

	if (InputMgr.GetState(P2_BUTTON_4))
	{
		Byte |= 0x40;
	}

	if (InputMgr.GetState(P2_BUTTON_5))
	{
		Byte |= 0x20;
	}

	if (InputMgr.GetState(P2_BUTTON_6))
	{
		Byte |= 0x10;
	}

	return Byte;
}