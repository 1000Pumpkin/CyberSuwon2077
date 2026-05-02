#include "pch.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR cmdLine, int cmdShow)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInst;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_CLASSDC;
    wc.lpszClassName = "CS";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(WS_EX_LEFT, "CS", "CyberSUWON", WS_POPUP, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, cmdShow);
    UpdateWindow(hWnd);

    if (Direct3D::GetInstance()->Initialize(hWnd))
    {
        RankingManager::GetInstance()->Load();
        while (true)
        {
            MSG msg;
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                Time::GetInstance()->Tick([](float deltaTime) {
                    Direct3D::GetInstance()->Update(deltaTime);
                    });
                Direct3D::GetInstance()->Render();
            }
        }
        RankingManager::GetInstance()->Save();
    }

    return 0;
}