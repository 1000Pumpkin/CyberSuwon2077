#pragma once
class Sprite
{
public:
    vector<Texture*> pTexs;
    int curScene = 0;
    float aniTime = 0.05f;
    float curAniTime = 0.f;
    bool isLoop = false;
    float maskRate = 1.f;

    float a = 1.f, r = 1.f, g = 1.f, b = 1.f;

    D3DXVECTOR2 pos = D3DXVECTOR2(0, 0);
    D3DXVECTOR2 scale = D3DXVECTOR2(1, 1);
    D3DXVECTOR2 center = D3DXVECTOR2(0.5f, 0.5f);
    D3DXVECTOR2 scaleAnchor = D3DXVECTOR2(0, 0);
    D3DXVECTOR2 rotateAnchor = D3DXVECTOR2(0, 0);
    float rotate = 0.f;

    Sprite() {};
    Sprite(string path);

    void Update(float deltaTime);
    void Render();

    float GetTextureWidth();
    float GetTextureHeight();
};

