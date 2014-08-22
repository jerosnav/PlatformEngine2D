#pragma once

#include <irrlicht.h>

using namespace irr;

class EventReceiverMgr;

class InputMouse
{
public:

	friend class EventReceiverMgr;

	static void Reset()
	{
		LeftKeyIsDown = false;
		AsyncLeftKeyPressed = false;
		AsyncLeftKeyReleased = false;
		SyncLeftKeyPressed = false;
		SyncLeftKeyReleased = false;
	}

	static core::position2di GetPosition()
	{
		return Position;
	}

	// This is used to check whether a key is being held down
	static bool IsLeftKeyDown()
	{
		return LeftKeyIsDown;
	}

	// If key was pressed las frame
	static bool IsLeftKeyPressed( )
	{
		return SyncLeftKeyPressed;
	}

	// If key was released las frame
	static bool IsLeftKeyReleased( )
	{
		return SyncLeftKeyReleased;
	}

	// Call every update
	static void Update( )
	{
		SyncLeftKeyPressed = AsyncLeftKeyPressed;
		AsyncLeftKeyPressed = false;
		SyncLeftKeyReleased = AsyncLeftKeyReleased;
		AsyncLeftKeyReleased = false;
	}

private:

	static core::position2di Position;

	// We use this array to store the current state of each key
	static bool LeftKeyIsDown;

	// Keep pressed state
	static bool AsyncLeftKeyPressed;

	// keep released state
	static bool AsyncLeftKeyReleased;

	// Sync state
	static bool SyncLeftKeyPressed;
	static bool SyncLeftKeyReleased;
};