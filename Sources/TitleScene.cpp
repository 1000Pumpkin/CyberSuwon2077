#include "pch.h"

void TitleScene::Init()
{
    PlayerManager::GetInstance()->gameBgm->Stop();
    PlayerManager::GetInstance()->titleBgm->Play(false);
    sprite = Sprite("Assets/main/0.png");
    sprite.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    ButtonInfo bInfo;
    bInfo.name = "start";
    bInfo.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, 320);
    bInfo.type = ButtonType::bstart;
    AddObject(new Button(bInfo));
    bInfo.name = "howto";
    bInfo.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, 240);
    bInfo.type = ButtonType::bhowto;
    AddObject(new Button(bInfo));
    bInfo.name = "rank";
    bInfo.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, 160);
    bInfo.type = ButtonType::brank;
    AddObject(new Button(bInfo));
    bInfo.name = "exit";
    bInfo.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, 80);
    bInfo.type = ButtonType::bexit;
    AddObject(new Button(bInfo));
}

void TitleScene::Term()
{
}

void TitleScene::Render()
{
    sprite.Render();
    Scene::Render();
}
