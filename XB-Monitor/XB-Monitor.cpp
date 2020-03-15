// XB-Monitor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

// This is an example of an exported variable
TTX_MONITOR_API int nXB-Monitor=0;

// This is an example of an exported function.
TTX_MONITOR_API int fnXB-Monitor(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see XB-Monitor.h for the class definition
CXB-Monitor::CXB-Monitor()
{
	return;
}
