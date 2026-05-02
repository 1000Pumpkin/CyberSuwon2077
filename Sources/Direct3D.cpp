#include "pch.h"

bool Direct3D::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D9)
        return false;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = true;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

    if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3D9Device)))
        return false;

    pD3D9Device->SetRenderState(D3DRS_ZENABLE, true);
    pD3D9Device->SetRenderState(D3DRS_LIGHTING, false);
    pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

    pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pD3D9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    D3DXMatrixOrthoOffCenterLH(&projMat, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000);
    pD3D9Device->SetTransform(D3DTS_PROJECTION, &projMat);

    HRESULT hr = pD3D9Device->CreateVertexBuffer(sizeof(Vertex) * 4, 0, VertexFVF, D3DPOOL_DEFAULT, &pVB, NULL);
    if (FAILED(hr))
        return false;

    hr = D3DXCreateLine(pD3D9Device, &pLine);
    if (FAILED(hr))
        return false;

    D3DXCreateFontIndirect(pD3D9Device, new D3DXFONT_DESC{
        90, 90,
        FW_BOLD, 1, FALSE,
        DEFAULT_CHARSET,
        OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE,
        "Small Fonts"
        }, &pFont);

    MoveCamera(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

    SceneManager::GetInstance()->AddScene("Editor", new EditorScene);
    SceneManager::GetInstance()->AddScene("Loading", new LoadingScene);
    SceneManager::GetInstance()->AddScene("Title", new TitleScene);
    SceneManager::GetInstance()->AddScene("HowTo", new HowToScene);
    SceneManager::GetInstance()->AddScene("Ranking", new RankingScene);
    SceneManager::GetInstance()->AddScene("Stage0", new GameScene(0));
    SceneManager::GetInstance()->AddScene("Stage1", new GameScene(1));
    SceneManager::GetInstance()->AddScene("Result", new ResultScene);
    SceneManager::GetInstance()->ChangeScene("Loading");

    return true;
}

void Direct3D::Update(float deltaTime)
{
    D3DXMatrixOrthoOffCenterLH(&projMat,
        -(SCREEN_WIDTH * zoom - SCREEN_WIDTH) / 2,
        SCREEN_WIDTH + (SCREEN_WIDTH * zoom - SCREEN_WIDTH) / 2,
        -(SCREEN_HEIGHT * zoom - SCREEN_HEIGHT) / 2,
        SCREEN_HEIGHT + (SCREEN_HEIGHT * zoom - SCREEN_HEIGHT) / 2,
        0, 1000);
    pD3D9Device->SetTransform(D3DTS_PROJECTION, &projMat);

    KeyManager::GetInstance()->Update();
    SceneManager::GetInstance()->CheckChangeScene();
    SceneManager::GetInstance()->Update(deltaTime);
    static int frameCount = 0;
    frameCount++;
}

void Direct3D::Render()
{
    pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, -1, 0);
    pD3D9Device->BeginScene();

    SceneManager::GetInstance()->Render();

    pD3D9Device->EndScene();
    pD3D9Device->Present(0, 0, 0, 0);
}

void Direct3D::MoveCamera(D3DXVECTOR2 pos)
{
    D3DXVECTOR3 eye = D3DXVECTOR3(pos.x, pos.y, 0) - D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
    D3DXVECTOR3 at = D3DXVECTOR3(pos.x, pos.y, 0) - D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -5);
    D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

    D3DXMatrixLookAtLH(&viewMat, &eye, &at, &up);
    pD3D9Device->SetTransform(D3DTS_VIEW, &viewMat);

    camPos = pos;
}
