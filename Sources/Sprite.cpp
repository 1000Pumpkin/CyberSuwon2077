#include "pch.h"

Sprite::Sprite(string path)
{
    _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
        return;

    path.erase(path.begin() + path.rfind("/") + 1, path.end());

    do
    {
        pTexs.push_back(TextureManager::GetInstance()->GetTexture(path + fd.name));
    } while (_findnext(handle, &fd) == 0);
    _findclose(handle);
}

void Sprite::Update(float deltaTime)
{
    curAniTime += deltaTime;

    if (curAniTime >= aniTime)
    {
        curAniTime -= aniTime;
        if (++curScene >= pTexs.size())
        {
            if (isLoop)
                curScene = 0;
            else
                curScene -= 1;
        }
    }
}

void Sprite::Render()
{
    auto pTex = pTexs[curScene];

    void* ppVB;
    Direct3D::GetInstance()->pVB->Lock(0, 0, &ppVB, 0);
    auto color = D3DCOLOR_ARGB(int(a * 255), int(r * 255), int(g * 255), int(b * 255));

    Direct3D::Vertex vertex[4];
    vertex[0] = Direct3D::Vertex{ D3DXVECTOR3(0, 0, 0), color, D3DXVECTOR2(0, 1) };
    vertex[1] = Direct3D::Vertex{ D3DXVECTOR3(0, GetTextureHeight(), 0), color, D3DXVECTOR2(0, 0) };
    vertex[2] = Direct3D::Vertex{ D3DXVECTOR3(GetTextureWidth() * maskRate, 0, 0), color, D3DXVECTOR2(maskRate, 1) };
    vertex[3] = Direct3D::Vertex{ D3DXVECTOR3(GetTextureWidth() * maskRate, GetTextureHeight(), 0), color, D3DXVECTOR2(maskRate, 0) };

    memcpy(ppVB, vertex, sizeof(vertex));
    Direct3D::GetInstance()->pVB->Unlock();

    D3DXVECTOR2 _tex = D3DXVECTOR2(GetTextureWidth(), GetTextureHeight());
    D3DXVECTOR2 _center = D3DXVECTOR2(center.x * _tex.x, center.y * _tex.y);
    D3DXVECTOR2 _scaleAnchor = D3DXVECTOR2(scaleAnchor.x * _tex.x, scaleAnchor.y * _tex.y) + _center;
    D3DXVECTOR2 _rotateAnchor = D3DXVECTOR2(rotateAnchor.x * _tex.x, rotateAnchor.y * _tex.y) + _center;
    D3DXVECTOR2 _pos = pos - _center;
    D3DXMATRIX matrix;
    auto device = Direct3D::GetInstance()->pD3D9Device;

    D3DXMatrixTransformation2D(&matrix, &_scaleAnchor, 0, &scale, &_rotateAnchor, D3DXToRadian(rotate), &_pos);
    device->SetTransform(D3DTS_WORLD, &matrix);

    device->SetTexture(0, pTex->pImage);
    device->SetFVF(Direct3D::GetInstance()->VertexFVF);
    device->SetStreamSource(0, Direct3D::GetInstance()->pVB, 0, sizeof(Direct3D::Vertex));

    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

float Sprite::GetTextureWidth()
{
    return pTexs[curScene]->info.Width;
}

float Sprite::GetTextureHeight()
{
    return pTexs[curScene]->info.Height;
}
