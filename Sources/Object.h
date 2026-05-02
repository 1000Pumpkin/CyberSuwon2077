#pragma once
#define COLLISION_BOX 0b001
#define COLLISION_LINE 0b010

class Object
{
public:
    D3DXVECTOR2 position = D3DXVECTOR2(0, 0);
    int layer = 0;

    int collisionLayer = 0;
    vector<FRECT> collisionMasks;
    vector<CLine> lineMasks;
    int collisionMode = 0;

    bool isRemove = false;

    string tag = "";

    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void OnCollision(Object* another, float deltaTime) = 0;
    virtual void LateUpdate(float deltaTime) = 0;
    virtual void Render();

    void SetMask(Sprite& sprite);
    void SetMask(FRECT rc);
    void SetLine(CLine line);
};

