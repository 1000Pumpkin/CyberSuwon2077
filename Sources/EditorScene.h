#pragma once
class EditorScene : public Scene
{
    vector<EnemySpawnObject*> spawnPlan;
    vector<EnemyMovePath> pathes;
    EnemySpawnObject* lastSpawn = nullptr;
    float spawnDeltaTime = 3.f;
    float loadSpawnDeltaTime = 0.f;

    int cursorType = -1;
    Sprite cursorSprite;
public:
    void Save();
    void Load();
    void NextCursor();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
};

