#include "pch.h"
#include "Character.h"

void Bullet::Init()
{
    char buffer[255];
    sprintf(buffer, "Assets/bullet/%s/*.png", info.name.c_str());
    sprite = Sprite(buffer);

    SetMask(sprite);

    if (info.moveDir != D3DXVECTOR2(0, 0))
    {
        D3DXVec2Normalize(&info.moveDir, &info.moveDir);
    }
    position = info.pos;
    collisionLayer = info.collisionLayer;
    tag = "Bullet";
    layer = 6;

    if (info.name == "laser")
    {
        sprite.aniTime = 0.15f;
        sprite.scale = D3DXVECTOR2(1, 1.5f);
        collisionMasks[0].ResizeHeight(0.f);
    }
    else
    {
        collisionMasks[0].Resize(0.7f);
    }

    nextRotate = rotate = GETANGLE(0, 0, info.moveDir.x, info.moveDir.y);
}

void Bullet::Term()
{
}

void Bullet::Update(float deltaTime)
{
    if (info.caster)
    {
        if (info.caster->isDead)
            isRemove = true;
    }

    if (info.name == "laser" && sprite.curScene == 6)
    {
        collisionLayer = 2;
        SetMask(sprite);
        collisionMasks[0].ResizeHeight(0.15f);
    }
    aliveTime += deltaTime;
    if (info.name == "laser" && aliveTime >= 3.f)
    {
        isRemove = true;
    }

    if (position.x > 1380 ||
        position.x < -100 ||
        position.y > 820 ||
        position.y < -100)
        isRemove = true;

    if (nextRotate != rotate)
    {
        rotate += (nextRotate - rotate) * 0.5f;
    }
    info.moveDir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
    position += info.moveDir * info.moveSpeed * deltaTime;
    sprite.Update(deltaTime);
}

void Bullet::OnCollision(Object* another, float deltaTime)
{
    if (another->tag == "Character")
    {
        auto character = static_cast<Character*>(another);

        if (character->isImmotal || character->isHit)
            return;

        if (info.name != "laser")
        {
            isRemove = true;

            EffectInfo effInfo;
            effInfo.aniTime = 0.f;
            effInfo.duration = 0.f;
            effInfo.name = info.boomEffName;
            effInfo.pos = position + info.moveDir * sprite.GetTextureWidth() * 0.5f;
            effInfo.rotate = sprite.rotate;
            effInfo.victim = another;
            SceneManager::GetInstance()->AddObject(new Effect(effInfo));
        }

        character->OnHit(info.damage);
    }
    if (another->tag == "Reflect")
    {
        if (info.name == "laser")
            return;

        Skill* skill = static_cast<Skill*>(another);
        info.damage = skill->caster->status.attack;

        collisionLayer = another->collisionLayer;

        nextRotate = GETANGLE(0, 0, info.moveDir.x, info.moveDir.y);

        if (nextRotate < 0)
            nextRotate += 180;
        else
            nextRotate -= 180;
    }
    if (another->tag == "Shield" || another->tag == "Bomb")
    {
        if (info.name == "laser")
            return;
        isRemove = true;
        EffectInfo effInfo;
        effInfo.aniTime = 0.f;
        effInfo.duration = 0.f;
        effInfo.name = "explode1";
        effInfo.pos = position + D3DXVECTOR2(Random::GetInstance()->rand(-10, 10), Random::GetInstance()->rand(-10, 10));
        effInfo.rotate = 0.f;
        SceneManager::GetInstance()->AddObject(new Effect(effInfo));
    }
}

void Bullet::LateUpdate(float deltaTime)
{
}

void Bullet::Render()
{
    sprite.pos = position;
    sprite.rotate = rotate;
    sprite.Render();
#ifdef _DEBUG
    Object::Render();
#endif
}
