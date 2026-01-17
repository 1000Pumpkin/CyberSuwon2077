#pragma once

class EffectInfo
{
public:
    string name;
    float rotate;
    float duration;
    float aniTime;
    D3DXVECTOR2 pos;
    D3DXVECTOR2 scale = D3DXVECTOR2(1, 1);
    Object* victim = nullptr;
};

class Effect : public Object
{
public:
    float aliveTime = 0.f;
    D3DXVECTOR2 correctionPos;
    Sprite sprite;
    EffectInfo info;
    Effect(EffectInfo info) { this->info = info; }

    // Object을(를) 통해 상속됨
    virtual void Init();
    virtual void Term();
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime);
    virtual void LateUpdate(float deltaTime);
    virtual void Render();
};

class MoveEffect : public Effect
{
public:
    MoveEffect(EffectInfo info) : Effect(info) {};

    virtual void Update(float deltaTime);
};

class DamageEffect : public Effect
{
public:
    vector<float> numScales;
    int numCount = 0;
    int damage = 0;
    DamageEffect(EffectInfo info, int damage) : Effect(info) { this->damage = damage; }

    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Render();
};