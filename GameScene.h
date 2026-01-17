#pragma once
class GameScene : public Scene
{
    int stage;
    float gameTime = 0.f;
    float gameTimeStop = 0.f;
    bool stageClear = false;
    float clearTime = 0.f;
    int hpScore = 0;
    Sprite clearSprite;
    Sprite pauseSprite;
    bool isF7 = false;
    float timer = 0.f;
    bool isF9 = false;

    vector<EnemySpawnObject*> spawnPlan;
    vector<Enemy*> bossEnemies;
public:

    GameScene(int stage) { this->stage = stage; }

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime);
    virtual void Render();
};

