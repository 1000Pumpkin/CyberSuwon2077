#include "pch.h"

const int Status::maxLevel = 5;

void Character::Init()
{
    tag = "Character";
}

void Character::Term()
{
}

void Character::Update(float deltaTime)
{
    if (moveDir != D3DXVECTOR2(0, 0))
    {
        realPosition += moveDir * status.moveSpeed * deltaTime;
        moveDir = D3DXVECTOR2(0, 0);
    }
    position = realPosition;

    if (isHit)
    {
        hitDeltaTime += deltaTime;
        if (hitDeltaTime >= hitDuration)
        {
            hitDeltaTime = 0.f;
            isHit = false;
        }
    }

    if (status.hp <= 0)
    {
        isDead = true;
        status.hp = 0;
    }

    r += deltaTime;
    if (r >= 1.f)
        r = 1.f;
    g += deltaTime;
    if (g >= 1.f)
        g = 1.f;
    b += deltaTime;
    if (b >= 1.f)
        b = 1.f;

    levitation += deltaTime;

    isImmotal = false;
    for (auto& iter : skills)
    {
        if (iter->state == Skill::State::Cast)
            isImmotal |= iter->immotal;
    }

    status.Update();
}

void Character::OnCollision(Object* another, float deltaTime)
{
}

void Character::LateUpdate(float deltaTime)
{
}

void Character::OnHit(int damage)
{
    b = 0.5f;
    g = 0.5f;
    isHit = true;
    status.hp -= damage;
    
    EffectInfo effInfo;
    effInfo.aniTime = 0.f;
    effInfo.duration = 1.f;
    effInfo.name = "";
    effInfo.pos = position;
    effInfo.victim = this;
    effInfo.rotate = 0.f;
    SceneManager::GetInstance()->AddObject(new DamageEffect(effInfo, damage));
    //PlayerManager::GetInstance()->hitSound->Play(false);
}
