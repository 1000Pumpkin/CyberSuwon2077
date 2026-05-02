#pragma once

class GaugeUI
{
public:
    Sprite baseSprite;
    float rate = 1.f;

    GaugeUI(string path, D3DXVECTOR2 pos);
    void Render();
};

class ScoreUI
{
public:
    int score = 0;
    int prevScore = 0;
    Sprite scoreSprite;
    int numCount = 8;
    vector<float> numScales;

    ScoreUI(string path, D3DXVECTOR2 pos);
    void Update(float deltaTime);
    void Render();
};

class SkillUI
{
public:
    GaugeUI coolGauge;
    const Skill* skill;
    SkillUI(const Skill* skill, D3DXVECTOR2 pos);

    void Render();
};

class UI : public Object
{
public:
    Sprite baseSprite;
    Sprite toggleSprite;
    Sprite msgSprite;
    D3DXVECTOR2 basePos = D3DXVECTOR2(30, SCREEN_HEIGHT - 142);
    D3DXVECTOR2 skillPos = D3DXVECTOR2(30, 30);
    vector<SkillUI> skills;
    GaugeUI expGauge;
    GaugeUI levelGauge;
    GaugeUI hpGauge;
    GaugeUI bossHpGauge;
    ScoreUI score;
    int playerLevel = 1;

    enum MessageType :int
    {
        cant_use_skill,
        item_get_hp,
        item_get_resetcool,
        item_get_levelup,
        levelup,
        stg1_start,
        stg2_start
    };

    UI();   
    void AppearMessage(MessageType type);
    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

extern UI* g_pUI;