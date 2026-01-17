#pragma once

class StatusInfo
{
public:
    int hp;
    int exp;
    int attack;
    int attackSpeed;
    float moveSpeed;
};

class Status
{
public:
    int level;
    int hp;
    int exp = 0;
    int attack;
    int attackSpeed;
    float moveSpeed;
    static const int maxLevel;

    StatusInfo info;

    Status() {};
    Status(StatusInfo info, int level)
    {
        this->info = info;
        this->level = level;

        hp = info.hp;
        attack = info.attack;
        attackSpeed = info.attackSpeed;
        moveSpeed = info.moveSpeed;

        for (int i = 1; i < level; ++i)
        { 
            hp += (int)ceil(hp * 1.25) - hp;
            attack += (int)ceil(attack * 1.25) - attack;
            attackSpeed += (int)ceil(attackSpeed * 1.25) - attackSpeed;
        }
    }

    int GetMaxExp(int level)
    {
        int maxExp = info.exp;
        for (int i = 1; i < level; ++i)
        {
            maxExp += (int)ceil(maxExp * 1.5);
        }
        return maxExp;
    }

    int GetMaxHp(int level)
    {
        int maxHp = info.hp;
        for (int i = 1; i < level; ++i)
        {
            maxHp += (int)ceil(maxHp * 1.25) - maxHp;
        }
        return maxHp;
    }

    void Update()
    {
        if (level >= maxLevel)
            return;

        if (exp >= GetMaxExp(level))
        {
            *this = Status(info, level + 1);
        }
    }
};

class Character : public Object
{
public:
    vector<Skill*> skills;
    Status status;
    StatusInfo statusInfo;

    D3DXVECTOR2 realPosition = D3DXVECTOR2(0, 0);
    float levitation = 0.f;
    D3DXVECTOR2 moveDir = D3DXVECTOR2(0, 0);

    bool isImmotal = false;
    bool isDead = false;
    bool isHit = false;
    float hitDuration;
    float hitDeltaTime = 0.f;

    float r = 1.f, g = 1.f, b = 1.f;

    // Object을(를) 통해 상속됨
    virtual void Init();
    virtual void Term();
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime);
    virtual void LateUpdate(float deltaTime);

    virtual void OnHit(int damage);
};

