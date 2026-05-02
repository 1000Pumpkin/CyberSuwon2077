#pragma once

class MapLayerInfo
{
public:
    D3DXVECTOR2 offset;
    int layer;
    float* gameTime;
    float mapWidth = 0.f;
    string mapPath;
    float scrollSpeed;
};

class MapLayer : public Object
{
public:
    MapLayerInfo info;
    Sprite sprite;
    int renderCount;
    vector<int> randomScenes;
    int prevMul = 0;
    MapLayer(MapLayerInfo info) { this->info = info; }

    // Object을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Object* another, float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;
    virtual void Render() override;
};

