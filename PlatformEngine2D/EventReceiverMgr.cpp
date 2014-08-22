#include "EventReceiverMgr.h"

bool EventReceiverMgr::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if( InputKeyboard::KeyIsDown[event.KeyInput.Key] != event.KeyInput.PressedDown )// avoid key repeat
		{
			InputKeyboard::KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			// register pressed state
			if( event.KeyInput.PressedDown )
			{
				InputKeyboard::AsyncKeyPressedState[event.KeyInput.Key] = event.KeyInput.PressedDown;
			}

			// register released state
			if( false == event.KeyInput.PressedDown )
			{
				InputKeyboard::AsyncKeyReleasedState[event.KeyInput.Key] = true;
			}
		}
	}
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				InputMouse::LeftKeyIsDown = true;
				InputMouse::AsyncLeftKeyPressed = true;
				break;

			case EMIE_LMOUSE_LEFT_UP:
				InputMouse::LeftKeyIsDown = false;
				InputMouse::AsyncLeftKeyReleased = true;
				break;

			case EMIE_MOUSE_MOVED:
				InputMouse::Position.X = event.MouseInput.X;
				InputMouse::Position.Y = event.MouseInput.Y;
				break;

			default:
				// We won't use the wheel
				break;
		}
	}

	return false;
}