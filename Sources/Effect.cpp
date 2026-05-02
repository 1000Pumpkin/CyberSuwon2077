#include "pch.h"

void Effect::Init()
{
    char buffer[255];
    sprintf(buffer, "Assets/effect/%s/*.png", info.name.c_str());
    sprite = Sprite(buffer);

    if (info.aniTime != 0.f)
        sprite.aniTime = info.aniTime;
    if (info.duration == 0.f)
        info.duration = sprite.aniTime * sprite.pTexs.size();
    sprite.scale = info.scale;
    if (info.victim)
        correctionPos = info.pos - info.victim->position;
    position = info.pos;
    layer = 20;
}

void Effect::Term()
{
}

void Effect::Update(float deltaTime)
{
    aliveTime += deltaTime;
    if (aliveTime >= info.duration)
        isRemove = true;

    if (info.victim)
        position = info.victim->position + correctionPos;

    sprite.Update(deltaTime);
}

void Effect::OnCollision(Object* another, float deltaTime)
{
}

void Effect::LateUpdate(float deltaTime)
{
}

void Effect::Render()
{
    sprite.pos = position;
    sprite.Render();
}

void MoveEffect::Update(float deltaTime)
{
    Effect::Update(deltaTime);

    position.x -= 50.f * deltaTime;
    sprite.scale.x -= deltaTime;
    sprite.scale.y -= deltaTime;

    if (sprite.scale.x <= 0.f)
        isRemove = true;
}

void DamageEffect::Init()
{
    sprite = Sprite("Assets/number/*.png");

    if (info.aniTime != 0.f)
        sprite.aniTime = info.aniTime;
    if (info.duration == 0.f)
        info.duration = sprite.aniTime * sprite.pTexs.size();
    sprite.scale = info.scale;
    if (info.victim)
        correctionPos = info.pos - info.victim->position;
    position = info.pos;
    layer = 20;

    int calcDamage = damage;
    while (true)
    {
        calcDamage /= 10;
        numCount++;
        if (calcDamage <= 0)
            break;
    }
    numScales.resize(numCount, 1.5f);
}

void DamageEffect::Update(float deltaTime)
{
    aliveTime += deltaTime;
    if (aliveTime >= info.duration)
        isRemove = true;
    for (auto& iter : numScales)
    {
        iter -= deltaTime;
        if (iter <= 1.f)
            iter = 1.f;
    }
    correctionPos.y += 50.f * deltaTime;
    if (info.victim)
        position = info.victim->position + correctionPos;
}

void DamageEffect::Render()
{
    int calcDamage = damage;
    float damageWidth = sprite.GetTextureWidth() + 5.f;
    for (int i = 0; i < numCount; ++i)
    {
        sprite.curScene = calcDamage % 10;
        calcDamage /= 10;
        sprite.pos = D3DXVECTOR2(position.x + damageWidth * (numCount - 1) - damageWidth * i, position.y);
        if (info.victim->collisionLayer == 1)
        {
            sprite.g = 0.5f;
        }
        if (info.victim->collisionLayer == 2)
        {
            sprite.g = 0.75f;
            sprite.b = 0.5f;
        }
        sprite.scale = numScales[i] * D3DXVECTOR2(1, 1);
        sprite.Render();
    }
}
