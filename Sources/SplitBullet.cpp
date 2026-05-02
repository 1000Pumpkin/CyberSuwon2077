#include "pch.h"

void SplitBullet::Init()
{
    sprite = Sprite("Assets/bullet/split/*.png");

    SetMask(sprite);

    if (info.moveDir != D3DXVECTOR2(0, 0))
    {
        D3DXVec2Normalize(&info.moveDir, &info.moveDir);
    }
    position = info.pos;
    collisionLayer = info.collisionLayer;
    tag = "Bullet";
    layer = 6;

    nextRotate = rotate = GETANGLE(0, 0, info.moveDir.x, info.moveDir.y);
}

void SplitBullet::Term()
{
}

void SplitBullet::Update(float deltaTime)
{
    if (info.caster)
    {
        if (info.caster->isDead)
            isRemove = true;
    }
    aliveTime += deltaTime;
    if (aliveTime >= 2.f)
    {
        isRemove = true;
        for (int i = 0; i < 8; ++i)
        {
            BulletInfo bInfo;
            bInfo.boomEffName = "hit";
            bInfo.collisionLayer = collisionLayer;
            bInfo.damage = info.damage;
            bInfo.moveSpeed = 400.f;
            bInfo.name = "enemy2";
            bInfo.pos = position;
            float angle = 45 * i + Random::GetInstance()->rand(-20, 20);
            D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(angle), ANGLETOY(angle));
            D3DXVec2Normalize(&dir, &dir);
            bInfo.moveDir = dir;
            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
        }
    }
    if (nextRotate != rotate)
    {
        rotate += (nextRotate - rotate) * 0.5f;
    }
    info.moveDir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
    position += info.moveDir * info.moveSpeed * deltaTime;
}

void SplitBullet::OnCollision(Object* another, float deltaTime)
{
    if (another->tag == "Character")
    {
        auto character = static_cast<Character*>(another);

        if (character->isImmotal || character->isHit)
            return;

        isRemove = true;

        EffectInfo effInfo;
        effInfo.aniTime = 0.f;
        effInfo.duration = 0.f;
        effInfo.name = info.boomEffName;
        effInfo.pos = position + info.moveDir * sprite.GetTextureWidth() * 0.5f;
        effInfo.rotate = sprite.rotate;
        effInfo.victim = another;
        SceneManager::GetInstance()->AddObject(new Effect(effInfo));

        character->OnHit(info.damage);
    }
    if (another->tag == "Reflect")
    {
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

void SplitBullet::LateUpdate(float deltaTime)
{
}

void SplitBullet::Render()
{
    sprite.pos = position;
    sprite.Render();
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG

}
