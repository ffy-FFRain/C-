// 图标替换作业.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "图标替换作业.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//*******************************
int startX = 200, startY = 100;
RECT rect;             
HBITMAP bufferBitmap;  //内存缓冲位图
HDC bufferDC;//内存缓冲设备
PAINTSTRUCT ps;
//*******************************
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLE));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CIRCLE));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_CIRCLE));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	HDC hdc = BeginPaint(hWnd, &ps);
	HBRUSH hbrush;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CREATE:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd,&rect);//创建内存缓冲设备及内存缓冲位图
		bufferDC = CreateCompatibleDC(hdc);
		bufferBitmap = CreateCompatibleBitmap(GetDC(hWnd), rect.right - rect.left, rect.bottom - rect.top);
		SelectObject(bufferDC, bufferBitmap);

		hbrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(bufferDC,&rect,hbrush);
		DeleteObject(hbrush);

		break;
	case WM_CLOSE:
		if (IDOK == MessageBox(NULL, L"你确定要退出吗",
			L"提示", MB_OKCANCEL | MB_ICONQUESTION)) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		DrawIcon(bufferDC,LOWORD(lParam),HIWORD(lParam),LoadIcon(hInst,MAKEINTRESOURCE(IDI_CIRCLE)));
		RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);

		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		break;
	case WM_MOUSEMOVE:
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		if (wParam == MK_LBUTTON) {
			DrawIcon(bufferDC, startX, startY, LoadIcon(hInst, MAKEINTRESOURCE(IDI_CIRCLE)));
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			startX = startX - 5;
		}
		if (wParam == VK_RIGHT)
		{
			startX = startX + 5;
		}
		if (wParam == VK_UP)
		{
			startY = startY - 5;
		}
		if (wParam == VK_DOWN)
		{
			startY = startY + 5;
		}
		DrawIcon(bufferDC,startX,startY,LoadIcon(hInst,MAKEINTRESOURCE(IDI_CIRCLE)));
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &rect);
		// 将内存设备中绘制的内容绘到屏幕上
		BitBlt(hdc, 0, 0,rect.right-rect.left,rect.bottom-rect.top,bufferDC,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;
    case WM_DESTROY:
		ReleaseDC(hWnd, bufferDC);
		DeleteObject(bufferBitmap);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
