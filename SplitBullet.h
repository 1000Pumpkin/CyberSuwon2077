#pragma once
class SplitBullet : public Object
{
public:
    BulletInfo info;
    Sprite sprite;
    float aliveTime = 0.f;
    float rotate = 0.f;
    float nextRotate = 0.f;

    SplitBullet(BulletInfo info) { this->info = info; }

    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

