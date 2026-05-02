#include "pch.h"
#include "Character.h"

Skill::Skill(const Character* caster)
{
    this->caster = caster;
    collisionLayer = caster->collisionLayer;
}

bool Skill::DoCast()
{
    if (caster->status.level < reqLevel)
        return false;

    if(state != Ready)
        return false;

    deltaTime = 0.f;
    state = Cast;
    OnCast();

    return true;
}

void Skill::Update(float deltaTime)
{
    this->deltaTime += deltaTime;
    switch (state)
    {
    case Cast:
    {
        if (this->deltaTime >= castingTime)
        {
            state = Cool;
            this->deltaTime = 0.f;
        }
    }break;
    case Cool:
    {
        if (this->deltaTime >= coolTime)
        {
            state = Ready;
            this->deltaTime = 0.f;
        }
    }break;
    }
}

void SkillShield::OnCast()
{
    position = caster->position;
}

void SkillShield::Init()
{
    sprite = Sprite("Assets/effect/shield/*.png");
    SetMask(sprite);

    immotal = true;
    reqLevel = 3;
    castingTime = 3.f;
    coolTime = 20.f;
    tag = "Shield";
}

void SkillShield::Update(float deltaTime)
{
    Skill::Update(deltaTime);

    collisionLayer = 0;

    if (state != Cast)
        return;

    collisionLayer = 1;
    position = caster->position;
}

void SkillShield::Render()
{
    if (state != Cast)
        return;
    sprite.pos = position;
    sprite.Render();
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG
}

void SkillReflect::OnCast()
{
    position = caster->position;
}

void SkillReflect::Init()
{
    sprite = Sprite("Assets/effect/reflect/*.png");
    SetMask(sprite);

    sprite.isLoop = true;
    immotal = true;
    reqLevel = 4;
    castingTime = 3.f;
    coolTime = 30.f;
    tag = "Reflect";
}

void SkillReflect::Update(float deltaTime)
{
    Skill::Update(deltaTime);

    collisionLayer = 0;

    if (state != Cast)
        return;

    collisionLayer = 1;
    position = caster->position;
    sprite.Update(deltaTime);
}

void SkillReflect::Render()
{
    if (state != Cast)
        return;
    sprite.pos = position;
    sprite.Render();
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG

}

void SkillBomb::OnCast()
{
    position = startPos = caster->position;
}

void SkillBomb::Init()
{
    sprite = Sprite("Assets/bullet/bomb/*.png");

    immotal = false;
    reqLevel = 5;
    castingTime = 5.f;
    coolTime = 40.f;
    tag = "Bomb";

    boomMask = FRECT{ -400,400,400,-400 };
}

void SkillBomb::Update(float deltaTime)
{
    Skill::Update(deltaTime);

    collisionLayer = 0;
    collisionMasks.clear();

    if (state != Cast)
        return;

    SetMask(sprite);
    collisionLayer = 1;

    auto diffPos = startPos - position;
    if (D3DXVec2Length(&diffPos) > 200.f)
    {
        static float effIntervalTime = 0.1f;
        static float effDeltaTime = effIntervalTime;

        effDeltaTime += deltaTime;

        if (effDeltaTime >= effIntervalTime)
        {
            EffectInfo effInfo;
            effInfo.aniTime = 0.01f;
            effInfo.duration = 0.f;
            effInfo.name = "explode3";
            for (int i = 0; i < 6; ++i)
            {
                effInfo.pos = position + D3DXVECTOR2(Random::GetInstance()->rand(-400, 400), Random::GetInstance()->rand(-400, 400));
                effInfo.rotate = Random::GetInstance()->rand(0, 360);
                SceneManager::GetInstance()->AddObject(new Effect(effInfo));
            }
            effDeltaTime = 0.f;
        }
        SetMask(boomMask);
    }
    else
    {
        position.x += 200.f * deltaTime;
    }
}

void SkillBomb::OnCollision(Object* another, float deltaTime)
{
    if (state != Cast)
        return;

    static float hitIntervalTime = 0.1f;
    static float hitDeltaTime = hitIntervalTime;

    hitDeltaTime += deltaTime;
    if (hitDeltaTime >= hitIntervalTime)
    {
        if (another->tag == "Character")
        {
            auto character = static_cast<Character*>(another);

            character->OnHit(caster->status.attack);
        }
        hitDeltaTime = 0.f;
    }
}

void SkillBomb::Render()
{
    if (state != Cast)
        return;

    auto diffPos = position - startPos;
    if (D3DXVec2Length(&diffPos) < 200)
    {
        sprite.pos = position;
        sprite.Render();
    }
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG

}
