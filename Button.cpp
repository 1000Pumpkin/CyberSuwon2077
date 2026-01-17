#include "pch.h"

Button::Button(ButtonInfo info)
{
    this->info = info;
}

void Button::Init()
{
    sprite = Sprite("Assets/main/menu/" + info.name + "/*.png");
    position = info.pos;
}

void Button::Term()
{
}

void Button::Update(float deltaTime)
{
    D3DXVECTOR2 cursorPos = KeyManager::GetInstance()->GetCursorPosition() + Direct3D::GetInstance()->camPos;

    D3DXVECTOR2 imageSize = D3DXVECTOR2(sprite.GetTextureWidth() / 2, sprite.GetTextureHeight() / 2);
    if (cursorPos.x >= position.x - imageSize.x &&
        cursorPos.x <= position.x + imageSize.x &&
        cursorPos.y >= position.y - imageSize.y &&
        cursorPos.y <= position.y + imageSize.y)
    {
        isOn = true;
    }
    else
    {
        isOn = false;
    }

    if (isOn && KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_UP)
    {
        switch (info.type)
        {
        case ButtonType::bstart:
        {
            SceneManager::GetInstance()->ChangeScene("Stage0");
        }break;
        case ButtonType::bhowto:
        {
            SceneManager::GetInstance()->ChangeScene("HowTo");
        }break;
        case ButtonType::brank:
        {
            SceneManager::GetInstance()->ChangeScene("Ranking");
        }break;
        case ButtonType::bexit:
        {
            PlayerManager::GetInstance()->Term();
            DestroyWindow(Direct3D::GetInstance()->m_hWnd);
            //PostQuitMessage(0);
        }break;
        }
    }
}

void Button::OnCollision(Object* another, float deltaTime)
{
}

void Button::LateUpdate(float deltaTime)
{
}

void Button::Render()
{
    if (isOn)
        sprite.curScene = 1;
    else
        sprite.curScene = 0;
    sprite.pos = position;
    sprite.Render();
}
