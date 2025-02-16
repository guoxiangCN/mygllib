#define _CRT_SECURE_NO_WARNINGS

#include "mygllib.h"

#include <math.h>
#include <stdio.h>

#include "assert.h"
#include "framework.h"
#include "mygllib_canvas.h"
#include "mygllib_image.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

HWND hWnd;
int wWidth = 1920;
int wHeight = 1080;
HDC hDC;
HDC hMem;
mygl::Canvas* gPCanvas = nullptr;
mygl::Image* gSampleImage = nullptr;

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

VOID Render();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    freopen("D:\\mygllib-output.txt", "w", stdout);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYGLLIB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable =
        LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGLLIB));

    // 创建绘图用的位图
    void* buffer = 0;

    hDC = GetDC(hWnd);
    hMem = ::CreateCompatibleDC(hDC);

    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = wWidth;
    bmpInfo.bmiHeader.biHeight = wHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;  // 实际存储为bgr
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    HBITMAP hBmp =
        CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
    assert(hBmp != nullptr);

    SelectObject(hMem, hBmp);

    memset(buffer, 0, wWidth * wHeight * 4);

    // Init Canvas
    gPCanvas = new mygl::Canvas(wWidth, wHeight, buffer);

    // 初始化一张示例图片
    gSampleImage = mygl::ReadImageFromFile("C:\\Users\\mzygd\\Pictures\\1.png");

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Render();
    }

    return (int)msg.wParam;
}

VOID Render() {
    gPCanvas->Clear();

    // 绘制图片
    gPCanvas->DrawImage(10, 10, gSampleImage);

    // 老旧电视机雪花
    if (false) {
        for (int x = 0; x < wWidth; x++) {
            for (int y = 0; y < wHeight; y++) {
                mygl::RGBA color(rand() % 255, rand() % 255, rand() % 255,
                                 rand() % 255);
                gPCanvas->DrawPoint(mygl::Point2D(x, y), color);
            }
        }
    }

    // 度数转换为弧度
#define DEG2RAD(degrees) ((degrees) * 3.14159265358979323846 / 180.0)

    mygl::RGBA color(255, 0, 0, 255);

    // v1
    if (0) {
        mygl::Point2D pt1(200, 200);
        double dis = 50;  // r
        for (double i = 0; i < 360; i += 20) {
            double radian = DEG2RAD(i);
            int x = static_cast<int>(dis * cos(radian) + pt1.x);
            int y = static_cast<int>(dis * sin(radian) + pt1.y);

            // printf("x=%d y=%d \n", x, y);

            mygl::Point2D pt2(x, y);
            gPCanvas->DrawLineV1(pt1, pt2, color);
        }
    }
    // printf("====================================== \n");

    // v2
    if (0) {
        mygl::Point2D pt1(300, 300);
        double dis = 50;  // r
        for (double i = 0; i < 360; i += 20) {
            double radian = DEG2RAD(i);
            int x = static_cast<int>(dis * cos(radian) + pt1.x);
            int y = static_cast<int>(dis * sin(radian) + pt1.y);

            mygl::Point2D pt2(x, y);
            gPCanvas->DrawLineV2(pt1, pt2, color);
        }
    }

    // v3
    if (0) {
        mygl::Point2D pt1(400, 400);
        double dis = 50;  // r
        for (double i = 0; i < 360; i += 20) {
            double radian = DEG2RAD(i);
            int x = static_cast<int>(dis * cos(radian) + pt1.x);
            int y = static_cast<int>(dis * sin(radian) + pt1.y);

            mygl::Point2D pt2(x, y);
            gPCanvas->DrawLineV3(pt1, pt2, color);
        }
    }

    // 画到设备上
    BitBlt(hDC, 0, 0, wWidth, wHeight, hMem, 0, 0, SRCCOPY);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGLLIB));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;  // replace: MAKEINTRESOURCEW(IDC_MYGLLIB)
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;  // 将实例句柄存储在全局变量中

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP, CW_USEDEFAULT, 0,
                         wWidth, wHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    switch (message) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd,
                              About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
