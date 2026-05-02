#pragma once

enum KeyState : int 
{
    EKEY_DOWN,
    EKEY_UP,
    EKEY_PRESS,
    EKEY_NONE
};

class KeyManager : public Singleton<KeyManager>
{
    bool lastKeyState[256] = { false };
    int keyState[256] = { EKEY_NONE };
public:
    void Update();

    int GetKeyState(int key);

    D3DXVECTOR2 GetCursorPosition();
};

