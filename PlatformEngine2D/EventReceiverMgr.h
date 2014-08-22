#pragma once

#include <irrlicht.h>

#include "InputKeyboard.h"
#include "InputMouse.h"

using namespace irr;

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/
class EventReceiverMgr : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event);
	
	EventReceiverMgr()
	{
		InputKeyboard::Reset();
	}
};