#include "pch.h"

Item::Item(ItemInfo info)
{
    this->info = info;
}

void Item::Init()
{
    char buffer[255];
    sprintf(buffer, "Assets/item/%d.png", info.type);
    sprite = Sprite(buffer);

    SetMask(sprite);
    collisionMasks[0].Resize(1.5f);
    collisionLayer = 2;
    tag = "Item";

    moveDir = D3DXVECTOR2(Random::GetInstance()->rand(-50, 50) / 10.f, Random::GetInstance()->rand(-50, 50) / 10.f);
    if (moveDir.x == 0)
        moveDir.x = 1;
    if (moveDir.y == 0)
        moveDir.y = 1;
    position = info.pos;
}

void Item::Term()
{
}

void Item::Update(float deltaTime)
{
    aliveTime -= deltaTime;

    if (aliveTime <= 0.f)
        isRemove = true;

    position += moveDir * 50.f * deltaTime;

    D3DXVECTOR2 imageSize = D3DXVECTOR2(sprite.GetTextureWidth() / 2, sprite.GetTextureHeight() / 2);
    if (position.x < imageSize.x)
    {
        position.x = imageSize.x;
        moveDir.x *= -1;
    }
    if (position.x > SCREEN_WIDTH - imageSize.x)
    {
        position.x = SCREEN_WIDTH - imageSize.x;
        moveDir.x *= -1;
    }
    if (position.y < imageSize.y)
    {
        position.y = imageSize.y;
        moveDir.y *= -1;
    }
    if (position.y > SCREEN_HEIGHT - imageSize.y)
    {
        position.y = SCREEN_HEIGHT - imageSize.y;
        moveDir.y *= -1;
    }
}

void Item::OnCollision(Object* another, float deltaTime)
{
}

void Item::LateUpdate(float deltaTime)
{
}

void Item::Render()
{
    sprite.pos = position;
    sprite.Render();

#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG
}
