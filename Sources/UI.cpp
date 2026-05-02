#include "pch.h"

UI* g_pUI = nullptr;

GaugeUI::GaugeUI(string path, D3DXVECTOR2 pos)
{
    baseSprite = Sprite(path);
    baseSprite.center = D3DXVECTOR2(0, 0);
    baseSprite.pos = pos;
}

void GaugeUI::Render()
{
    baseSprite.maskRate = 1;
    baseSprite.curScene = 0;
    baseSprite.Render();
    baseSprite.maskRate = rate;
    baseSprite.curScene = 1;
    baseSprite.Render();
}

ScoreUI::ScoreUI(string path, D3DXVECTOR2 pos)
{
    scoreSprite = Sprite(path);
    scoreSprite.center = D3DXVECTOR2(0, 0);
    scoreSprite.pos = pos;
    numScales.resize(numCount, 1);
}

void ScoreUI::Update(float deltaTime)
{
    if (prevScore != score)
    {
        int calcScore = score;
        for (int i = 0; i < numCount; ++i)
        {
            if (calcScore % 10 != prevScore % 10)
            {
                numScales[i] = 1.5f;
            }
            calcScore /= 10;
            prevScore /= 10;
        }
        prevScore = score;
    }
    for (auto& iter : numScales)
    {
        iter -= deltaTime * 10.f;
        if (iter <= 1.f)
            iter = 1.f;
    }
}

void ScoreUI::Render()
{
    D3DXVECTOR2 pos = scoreSprite.pos;
    int scoreWidth = scoreSprite.GetTextureWidth() + 5.f;
    int calcScore = score;
    for (int i = 0; i < numCount; ++i)
    {
        scoreSprite.pos = D3DXVECTOR2(pos.x + scoreWidth * (numCount - 1) - scoreWidth * i, pos.y);
        scoreSprite.curScene = calcScore % 10;
        calcScore /= 10;
        scoreSprite.scale = D3DXVECTOR2(1, 1) * numScales[i];
        scoreSprite.Render();
    }
    scoreSprite.pos = pos;
}

SkillUI::SkillUI(const Skill* skill, D3DXVECTOR2 pos) : 
    coolGauge("Assets/ui/skill/" + skill->name + "/*.png", pos)
{
    this->skill = skill;
}

void SkillUI::Render()
{
    if (skill->caster->status.level < skill->reqLevel)
        coolGauge.rate = 0.f;
    else
    {
        switch (skill->state)
        {
        case Skill::State::Ready:
        {
            coolGauge.rate = 1.f;
        }break;
        case Skill::State::Cast:
        {
            coolGauge.rate = 1.f - skill->deltaTime / skill->castingTime;
        }break;
        case Skill::State::Cool:
        {
            coolGauge.rate = skill->deltaTime / skill->coolTime;
        }break;
        }
    }
    coolGauge.Render();
}

UI::UI() :
    expGauge("Assets/ui/exp/*.png", basePos + D3DXVECTOR2(135, 65)),
    levelGauge("Assets/ui/level/*.png", basePos + D3DXVECTOR2(135, 35)),
    hpGauge("Assets/ui/hp/*.png", basePos + D3DXVECTOR2(164, 5)),
    bossHpGauge("Assets/ui/hp/*.png", D3DXVECTOR2(1200, 360)),
    score("Assets/number/*.png", basePos + D3DXVECTOR2(135, 90))
{
    layer = 9999;
    baseSprite = Sprite("Assets/ui/base/*.png");
    baseSprite.center = D3DXVECTOR2(0, 0);
    baseSprite.pos = basePos;
    msgSprite = Sprite("Assets/ui/msg/*.png");
    msgSprite.a = 0.f;
    msgSprite.center = D3DXVECTOR2(0, 0);
    toggleSprite = Sprite("Assets/ui/toggle/*.png");
    toggleSprite.center = D3DXVECTOR2(0, 0);
    toggleSprite.pos = skillPos + D3DXVECTOR2(0, 120);
}

void UI::AppearMessage(MessageType type)
{
    msgSprite.curScene = type;
    msgSprite.a = 1.f;
}

void UI::Init()
{
    for (int i = 0; i < 3; ++i)
    {
        skills.push_back(SkillUI(PlayerManager::GetInstance()->g_pPlayer->skills[i], skillPos + D3DXVECTOR2(100 * i, 0)));
    }
    playerLevel = PlayerManager::GetInstance()->g_pPlayer->status.level;
}

void UI::Term()
{
}

void UI::Update(float deltaTime)
{
    auto player = PlayerManager::GetInstance()->g_pPlayer;
    msgSprite.a -= deltaTime;
    if (msgSprite.a <= 0.f)
        msgSprite.a = 0.f;

    if (player->status.level < Status::maxLevel)
        expGauge.rate = (float)player->status.exp / (float)player->status.GetMaxExp(player->status.level);
    else
        expGauge.rate = 1.f;

    if (playerLevel != player->status.level)
    {
        playerLevel = player->status.level;
        AppearMessage(levelup);
    }

    levelGauge.rate = (float)player->status.level / (float)Status::maxLevel;

    hpGauge.rate = (float)player->status.hp / (float)player->status.GetMaxHp(player->status.level);

    bossHpGauge.rate = PlayerManager::GetInstance()->bossHpRate;

    score.score = PlayerManager::GetInstance()->score;

    score.Update(deltaTime);
}

void UI::OnCollision(Object* another, float deltaTime)
{
}

void UI::LateUpdate(float deltaTime)
{
}

void UI::Render()
{
    if (msgSprite.curScene == levelup ||
        msgSprite.curScene == stg1_start ||
        msgSprite.curScene == stg2_start)
    {
        msgSprite.pos = D3DXVECTOR2(0, 0);
    }
    else
    {
        msgSprite.pos = skillPos + D3DXVECTOR2(320, -30);
    }
    baseSprite.Render();
    msgSprite.Render();
    toggleSprite.curScene = PlayerManager::GetInstance()->g_pPlayer->shotType;
    toggleSprite.Render();
    expGauge.Render();
    levelGauge.Render();
    hpGauge.Render();
    score.Render();
    if (PlayerManager::GetInstance()->isBossAppear)
    {
        bossHpGauge.baseSprite.rotate = 90.f;
        bossHpGauge.Render();
    }
    for (auto iter : skills)
        iter.Render();
}
