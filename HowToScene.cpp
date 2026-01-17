#include "pch.h"

void HowToScene::Init()
{
    sprite = Sprite("Assets/main/1.png");
    sprite.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void HowToScene::Term()
{
}

void HowToScene::Update(float deltaTime)
{
    if (KeyManager::GetInstance()->GetKeyState(VK_SPACE) == EKEY_UP ||
        KeyManager::GetInstance()->GetKeyState(VK_RETURN) == EKEY_UP ||
        KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_UP)
    {
        SceneManager::GetInstance()->ChangeScene("Title");
    }
}

void HowToScene::Render()
{
    sprite.Render();
}
