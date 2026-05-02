#pragma once
class LoadingScene : public Scene
{
public:
    Sprite sprite;
    vector<string> assetFiles;
    int curIndex = 0;

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Term() override;

    virtual void Update(float deltaTime) override;
    virtual void Render() override;

    vector<string> GetAssetFiles(string path);
};

