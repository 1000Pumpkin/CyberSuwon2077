#pragma once
class Direct3D : public Singleton<Direct3D>
{
public:
    struct Vertex
    {
        D3DXVECTOR3 pos;
        D3DCOLOR diffuse;
        D3DXVECTOR2 uvux;
    };

    const int VertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    IDirect3DVertexBuffer9* pVB;

    D3DXMATRIX projMat;
    D3DXMATRIX viewMat;

    IDirect3D9* pD3D9;
    IDirect3DDevice9* pD3D9Device;

    ID3DXLine* pLine;

    LPD3DXFONT pFont;

    D3DXVECTOR2 camPos;

    float zoom = 1.f;

    HWND m_hWnd;

    bool Initialize(HWND hWnd);
    void Update(float deltaTime);
    void Render();
    void MoveCamera(D3DXVECTOR2 pos);
};

