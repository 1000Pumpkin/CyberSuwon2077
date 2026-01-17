#pragma once

class Character;

class BulletInfo
{
public:
    D3DXVECTOR2 pos;
    D3DXVECTOR2 moveDir;
    string name;
    string boomEffName;
    int damage = 1;
    int collisionLayer;
    float moveSpeed;
    Character* caster = nullptr;
};

class Bullet : public Object
{
public:
    Sprite sprite;
    float rotate = 0.f;
    float nextRotate = 0.f;
    float aliveTime = 0.f;
    BulletInfo info;
    Bullet(BulletInfo info) { this->info = info; }

    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render();
};

