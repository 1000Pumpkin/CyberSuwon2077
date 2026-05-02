#pragma once

class EnemySpawnInfo
{
public:
    float spawnTime;
    float rotate;
    EnemyType type;
    vector<EnemyMovePath> pathes;
};

class EnemySpawnObject : public Object
{
    Enemy* enemy = nullptr;
public:
    EnemySpawnInfo info;

    EnemySpawnObject(EnemySpawnInfo info);
    EnemySpawnObject(ifstream& is);

    void Write(ofstream& os);

    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

