#pragma once

enum ButtonType : int
{
    bstart,
    bhowto,
    brank,
    bexit
};

class ButtonInfo
{
public:
    ButtonType type;
    string name;
    D3DXVECTOR2 pos;
};

class Button : public Object
{
public:
    Sprite sprite;
    ButtonInfo info;
    bool isOn = false;
    Button(ButtonInfo info);
    // ObjectÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

