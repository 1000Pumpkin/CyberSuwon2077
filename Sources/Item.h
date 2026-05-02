#pragma once

enum ItemType : int
{
    hp,
    resetcool,
    levelup
};

class ItemInfo
{
public:
    ItemType type;
    D3DXVECTOR2 pos;
};

class Item : public Object
{
public:
    Sprite sprite;
    ItemInfo info;
    D3DXVECTOR2 moveDir;
    float aliveTime = 10.f;

    Item(ItemInfo info);

    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

