#pragma once

#include <irrlicht.h>

using namespace irr;

class EventReceiverMgr;

class InputKeyboard
{
public:

	friend class EventReceiverMgr;

	static void Reset()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		{
			KeyIsDown[i] = false;
			AsyncKeyPressedState[i] = false;
			AsyncKeyReleasedState[i] = false;
			SyncKeyPressedState[i] = false;
			SyncKeyReleasedState[i] = false;
		}
	}

	// This is used to check whether a key is being held down
	static bool IsKeyDown(EKEY_CODE keyCode)
	{
		return KeyIsDown[keyCode];
	}

	// If key was pressed las frame
	static bool IsKeyPressed(EKEY_CODE keyCode)
	{
		return SyncKeyPressedState[keyCode];
	}

	// If key was released las frame
	static bool IsKeyReleased(EKEY_CODE keyCode)
	{
		return SyncKeyReleasedState[keyCode];
	}

	// Call every update
	static void Update( )
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		{
			SyncKeyPressedState[i] = AsyncKeyPressedState[i];
			AsyncKeyPressedState[i] = false;
			SyncKeyReleasedState[i] = AsyncKeyReleasedState[i];
			AsyncKeyReleasedState[i] = false;
		}
	}

private:

	// We use this array to store the current state of each key
	static bool KeyIsDown[KEY_KEY_CODES_COUNT];

	// Keep pressed state
	static bool AsyncKeyPressedState[KEY_KEY_CODES_COUNT];

	// keep released state
	static bool AsyncKeyReleasedState[KEY_KEY_CODES_COUNT];

	// Sync state
	static bool SyncKeyPressedState[KEY_KEY_CODES_COUNT];
	static bool SyncKeyReleasedState[KEY_KEY_CODES_COUNT];
};