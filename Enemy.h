#pragma once

class EnemyMovePath
{
public:
    D3DXVECTOR2 pos;
    bool isAttack;
};

enum EnemyType : int
{
    s1_t1,
    s1_t2,
    s1_mid,
    s1_boss,
    s2_t1,
    s2_t2,
    s2_mid,
    s2_boss,
    buoy,
    Count
};

class EnemyInfo
{
public:
    vector<EnemyMovePath> pathes;
    EnemyType type;
    float rotate;
};

class Enemy : public Character
{
public:
    EnemyInfo info;
    Sprite sprite;
    float moveEffTime = 0.f;
    int phase = 0;
    int attackType = 0;
    int pathIndex = 1;
    float bossLevitation = 0.f;
    float attackDeltaTime = 0.f;
    float attackIntervalTime = 0.f;
    int bulletCount = 0;
    float deadDeltaTime = 0.f;
    float deadEffDeltaTime = 0.f;

    static const string enemyTypeStrs[Count];
    static const bool enemyTypeIsBoss[Count];
    static const bool enemyTypeIsStageClear[Count];
    static const bool enemyTypeIsMoveEffect[Count];

    Enemy(EnemyInfo info);

    virtual void Init();
    virtual void Term();
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime);
    virtual void LateUpdate(float deltaTime);
    virtual void Render();
    virtual void OnHit(int damage);
};

