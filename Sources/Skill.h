#pragma once

class Character;

class Skill : public Object
{
public:
    enum State : int
    {
        Ready,
        Cast,
        Cool
    };

    const Character* caster;
    float deltaTime = 0.f;
    float castingTime;
    float coolTime;
    int reqLevel;
    string name;
    State state = Ready;
    bool immotal = false;

    Skill(const Character* caster);

    bool DoCast();
    virtual void OnCast() = 0;
    // Object을(를) 통해 상속됨
    virtual void Init() {};
    virtual void Term() {};
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime) {};
    virtual void LateUpdate(float deltaTime) {};
    virtual void Render() {};
};

class SkillShield : public Skill
{
public:
    Sprite sprite;
    SkillShield(const Character* caster) : Skill(caster) { name = "shield"; }

    virtual void OnCast();
    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Render();
};

class SkillReflect : public Skill
{
public:
    Sprite sprite;
    SkillReflect(const Character* caster) : Skill(caster) { name = "reflect"; }

    virtual void OnCast();
    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Render();
};

class SkillBomb : public Skill
{
public:
    D3DXVECTOR2 startPos;
    Sprite sprite;
    FRECT boomMask;
    SkillBomb(const Character* caster) : Skill(caster) { name = "bomb"; }

    virtual void OnCast();
    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime);
    virtual void Render();
};