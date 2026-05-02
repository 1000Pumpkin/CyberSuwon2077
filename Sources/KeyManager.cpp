#include "pch.h"

void KeyManager::Update()
{
    for (int i = 0; i < 256; ++i)
    {
        bool nowKeyState = (GetAsyncKeyState(i) & 0x8000) == 0x8000;
        bool _lastKeyState = lastKeyState[i];

        if (nowKeyState && _lastKeyState) keyState[i] = EKEY_DOWN;
        if (!nowKeyState && _lastKeyState) keyState[i] = EKEY_UP;
        if (nowKeyState && !_lastKeyState) keyState[i] = EKEY_PRESS;
        if (!nowKeyState && !_lastKeyState) keyState[i] = EKEY_NONE;

        lastKeyState[i] = nowKeyState;
    }
}

int KeyManager::GetKeyState(int key)
{
    return keyState[key];
}

D3DXVECTOR2 KeyManager::GetCursorPosition()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(Direct3D::GetInstance()->m_hWnd, &pt);
    pt.y = SCREEN_HEIGHT - pt.y;

    D3DXVECTOR2 gameResolution = D3DXVECTOR2(SCREEN_WIDTH * Direct3D::GetInstance()->zoom, SCREEN_HEIGHT * Direct3D::GetInstance()->zoom);
    D3DXVECTOR2 cursorPos;
    cursorPos.x = pt.x * Direct3D::GetInstance()->zoom - gameResolution.x / 2;
    cursorPos.y = pt.y * Direct3D::GetInstance()->zoom - gameResolution.y / 2;

    return cursorPos;
}
