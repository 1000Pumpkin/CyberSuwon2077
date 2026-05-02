#pragma once
class TitleScene : public Scene
{
public:
    Sprite sprite;
    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Render() override;
};

