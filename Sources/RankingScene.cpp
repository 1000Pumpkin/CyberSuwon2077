#include "pch.h"

void RankingScene::Init()
{
    sprite = Sprite("Assets/main/2.png");
    sprite.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void RankingScene::Term()
{
}

void RankingScene::Update(float deltaTime)
{
    if (PlayerManager::GetInstance()->isEnd)
    {
        if (RankingManager::GetInstance()->EditInitial())
        {
            if (KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_UP ||
                KeyManager::GetInstance()->GetKeyState(VK_SPACE) == EKEY_UP ||
                KeyManager::GetInstance()->GetKeyState(VK_RETURN) == EKEY_UP)
            {
                SceneManager::GetInstance()->ChangeScene("Title");
            }
        }
    }
    else
    {
        if (KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_UP ||
            KeyManager::GetInstance()->GetKeyState(VK_SPACE) == EKEY_UP ||
            KeyManager::GetInstance()->GetKeyState(VK_RETURN) == EKEY_UP)
        {
            SceneManager::GetInstance()->ChangeScene("Title");
        }
    }
}

void RankingScene::Render()
{
    sprite.Render();
    for (int i = 0; i < 3; ++i)
    {
        RankData rd;
        rd = RankingManager::GetInstance()->GetRankData(i);

        RECT rc = { 352, 300 + 135 * i, 518, 390 + 135 * i };
        Direct3D::GetInstance()->pFont->DrawTextA(NULL, rd.initial.c_str(), -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
        rc = { 565, 300 + 135 * i, 1280, 390 + 135 * i };
        char buffer[20];
        sprintf(buffer, "%08d", rd.score);
        Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
    }
}
