#include "InputMouse.h"

core::position2di InputMouse::Position;

// We use this array to store the current state of each key
bool InputMouse::LeftKeyIsDown;

// Keep pressed state
bool InputMouse::AsyncLeftKeyPressed;

// keep released state
bool InputMouse::AsyncLeftKeyReleased;

// Sync state
bool InputMouse::SyncLeftKeyPressed;
bool InputMouse::SyncLeftKeyReleased;