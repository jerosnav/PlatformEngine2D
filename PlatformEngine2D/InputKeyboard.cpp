#include "InputKeyboard.h"

bool InputKeyboard::KeyIsDown[KEY_KEY_CODES_COUNT];
bool InputKeyboard::AsyncKeyPressedState[KEY_KEY_CODES_COUNT];
bool InputKeyboard::AsyncKeyReleasedState[KEY_KEY_CODES_COUNT];
bool InputKeyboard::SyncKeyPressedState[KEY_KEY_CODES_COUNT];
bool InputKeyboard::SyncKeyReleasedState[KEY_KEY_CODES_COUNT];