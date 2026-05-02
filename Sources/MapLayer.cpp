#include "pch.h"

void MapLayer::Init()
{
    sprite = Sprite(info.mapPath);
    sprite.center = D3DXVECTOR2(0, 0);
    if (info.mapWidth == 0.f)
        info.mapWidth = sprite.GetTextureWidth();

    layer = info.layer;

    renderCount = (int)ceil(SCREEN_WIDTH / info.mapWidth) + 1;

    for (int i = 0; i < renderCount; ++i)
    {
        randomScenes.push_back(Random::GetInstance()->rand(0, sprite.pTexs.size() - 1));
    }
}

void MapLayer::Term()
{
}

void MapLayer::Update(float deltaTime)
{
    position = D3DXVECTOR2(*info.gameTime * -SCREEN_WIDTH * info.scrollSpeed, info.offset.y);

    int mul = (int)(position.x / info.mapWidth);

    if (prevMul != mul)
    {
        for (int i = 0; i < renderCount - 1; ++i)
        {
            randomScenes[i] = randomScenes[i + 1];
        }
        randomScenes[renderCount - 1] = Random::GetInstance()->rand(0, sprite.pTexs.size() - 1);
        prevMul = mul;
    }
    position.x -= info.mapWidth * mul;
}

void MapLayer::OnCollision(Object* another, float deltaTime)
{
}

void MapLayer::LateUpdate(float deltaTime)
{
}

void MapLayer::Render()
{
    for (int i = 0; i < renderCount; ++i)
    {
        sprite.pos = position + D3DXVECTOR2(info.mapWidth * i, 0);
        sprite.curScene = randomScenes[i];
        sprite.Render();
    }
}
