#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <cmath>
#pragma comment (lib,"Gdiplus.lib")

#include "obj/Worker.h"
#include "obj/Board.h"
#include "obj/Solder.h"
#include "obj/Automate.h"

using namespace Gdiplus;

#define TIMER_ID 1
#define TARGET_FPS 48

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Register the window class.
    const TCHAR CLASS_NAME[] = _T("Sample Window Class");

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(nullptr, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
        return 1;
    }

    // Create the window.
    HWND hWnd = CreateWindow(
        CLASS_NAME,
        _T("Solder"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        MessageBox(nullptr, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), NULL);
        return 1;
    }

    // Show the window.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Set up a timer to update the drawing at the specified FPS
    SetTimer(hWnd, TIMER_ID, 1000 / TARGET_FPS, nullptr);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Kill the timer
    KillTimer(hWnd, TIMER_ID);

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

Automate automate;
DWORD g_prevTick = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static float angle = 0.0f;
    static Gdiplus::Bitmap* buffer = nullptr;

    switch (message)
    {
    case WM_CREATE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        buffer = new Gdiplus::Bitmap(rect.right - rect.left, rect.bottom - rect.top, PixelFormat32bppARGB);
    }
    break;
    case WM_TIMER:
    {
        InvalidateRect(hWnd, nullptr, FALSE);
    }
    break;
    case WM_PAINT:
    {
        DWORD curTick = GetTickCount();
        if (g_prevTick == 0) {
            g_prevTick = curTick;
            break;
        }
        DWORD dt = curTick - g_prevTick;
        if (dt > 0) {
            g_prevTick = curTick;
        }
        else 
        g_prevTick = curTick;

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Graphics graphics(buffer);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
        graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

        graphics.Clear(Color(255, 255, 255, 255));

        RECT rect;
        GetClientRect(hWnd, &rect);

        automate.Update(dt, &graphics, rect.right - rect.left, rect.bottom - rect.top);

        Graphics screenGraphics(hdc);
        screenGraphics.DrawImage(buffer, 0, 0);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_SIZE:
    {
        if (buffer) delete buffer;
        RECT rect;
        GetClientRect(hWnd, &rect);
        buffer = new Gdiplus::Bitmap(rect.right - rect.left, rect.bottom - rect.top, PixelFormat32bppARGB);
    }
    break;
    case WM_DESTROY:
        delete buffer;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
