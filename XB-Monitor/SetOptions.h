// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#pragma once
#include <iostream>
#include <Windows.h>

#define MONITOR_MISC_FILENAME	"monitor_opt.bin"

typedef enum
{
	CONFIG_POVASAXIS,
	CONFIG_END,
} CONFIG;

class ConfigManager
{
	DWORD MiscTable[CONFIG_END];

public:
	ConfigManager();
	~ConfigManager();

	void Load();
	void LoadTable();
};