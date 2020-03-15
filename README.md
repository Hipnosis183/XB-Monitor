XB-Monitor+
-----------

XB-Monitor is a loader for Examu's eX-BOARD arcade machines software.
Originally developed by Romhack: https://github.com/zxmarcos/xb_monitor

Some features were added/changed/removed:

- Added support for AxisR, AxisZ and POVs
- Added option PovAsAxis
- Corrected the sensitivity of the Deadzone
- Added the standard Save Patch function
- Deactivated logging function (can be reactivated in code)
- Removed all DirectDraw stuff
- Fixed some bugs in Direct3D wrapper

The Config has been removed from the project, since the library is meant to be used with a series of WIP loaders that will replace it. Still, it can be used with the original Config program (but with the limited input mapping of it), or manual editing of the configuration files.