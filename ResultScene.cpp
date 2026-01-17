#include "pch.h"

void ResultScene::Init()
{
    PlayerManager::GetInstance()->gameBgm->Stop();
    if (PlayerManager::GetInstance()->isClear)
    {
        sprite = Sprite("Assets/result/0.png");
        PlayerManager::GetInstance()->winSound->Play(false);
    }
    else
    {
        sprite = Sprite("Assets/result/1.png");
        PlayerManager::GetInstance()->loseSound->Play(false);
    }
    sprite.pos = Direct3D::GetInstance()->camPos;
    PlayerManager::GetInstance()->isEnd = true;
}

void ResultScene::Term()
{
}

void ResultScene::Update(float deltaTime)
{
    if (KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_UP ||
        KeyManager::GetInstance()->GetKeyState(VK_SPACE) == EKEY_UP ||
        KeyManager::GetInstance()->GetKeyState(VK_RETURN) == EKEY_UP)
    {
        RankData rd;
        rd.initial = "___";
        rd.score = PlayerManager::GetInstance()->score;
        RankingManager::GetInstance()->Reset();
        RankingManager::GetInstance()->AddRanking(rd);
        SceneManager::GetInstance()->ChangeScene("Ranking");
    }
}

void ResultScene::Render()
{
    sprite.Render();

    char buffer[255];
    sprintf(buffer, "%08d", PlayerManager::GetInstance()->score);
    RECT rc = { 0, 360, 1280, 500 };
    Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
}
