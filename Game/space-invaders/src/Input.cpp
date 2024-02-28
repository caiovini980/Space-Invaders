#include "Input.h"

void Input::SetKeyStatus(const int key, const bool isPressed)
{
    m_Keys[key] = isPressed;
}

bool Input::GetKey(const int key) const
{
    return m_Keys[key];
}
