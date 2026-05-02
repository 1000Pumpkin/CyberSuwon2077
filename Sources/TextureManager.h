#pragma once

class Texture
{
public:
    IDirect3DTexture9* pImage;
    D3DXIMAGE_INFO info;
};

class TextureManager : public Singleton<TextureManager>
{
    map<string, Texture*> texMap;
public:
    Texture* GetTexture(string path)
    {
        auto tf = texMap.find(path);
        if (tf == texMap.end())
        {
            Texture* pTex = new Texture;

            if (FAILED(D3DXCreateTextureFromFileEx(
                Direct3D::GetInstance()->pD3D9Device,
                path.c_str(),
                D3DX_DEFAULT, D3DX_DEFAULT,
                1, 0,
                D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
                D3DX_DEFAULT, D3DX_DEFAULT,
                0, &pTex->info,
                NULL, &pTex->pImage
            )))
                return nullptr;
                
            tf = texMap.insert(make_pair(path, pTex)).first;
        }
        return tf->second;
    }
};