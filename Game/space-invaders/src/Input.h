#pragma once

class Input
{
public:
    void SetKeyStatus(int key, bool isPressed);
    bool GetKey(int key) const;
    
private:
    bool m_Keys[1024];

};
