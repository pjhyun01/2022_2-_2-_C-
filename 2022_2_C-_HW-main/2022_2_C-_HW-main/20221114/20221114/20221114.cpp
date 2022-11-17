// 20221114.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "20221114.h"
#include <thread>
#include <stdio.h>
#include <vector>
#include "platformtouch.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY20221114, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20221114));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20221114));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY20221114);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
RECT x;
RECT y;
RECT y_2;
RECT is;
BOOL p_jump;

int g_timer;
int z = 0;
int g_speed = 0;

BOOL KeyBuffer[256];

PLATFORM platform;

DWORD WINAPI moving(LPVOID param)
{
    if (KeyBuffer[VK_LEFT])
    {
        x.left -= 4;
        x.right -= 4;
    }

    if (KeyBuffer[VK_RIGHT])
    {
        x.left += 4;
        x.right += 4;
    }

    

    ExitThread(0);
    return(0);
}

DWORD WINAPI gravity(LPVOID param)
{
    
    if (KeyBuffer[VK_SHIFT] == FALSE)
    {
        if (false == IntersectRect(&is, &x, &y))
        {
            if (z == 0) 
            {
                z += 5;
            }
            else if (z <= 10)
            {
                z += 1;

            }
            x.top += z;
            x.bottom += z;
            //InvalidateRect(hWnd, NULL, true);
        }
        else if (true == IntersectRect(&is, &x, &y))
        {
            z = 0;
            p_jump = FALSE;
        }

        if (KeyBuffer[VK_SPACE])
        {
            if (p_jump == FALSE)
            {
                x.top -= 4;
                x.bottom -= 4;
                z = -16;
                p_jump = TRUE;
            }
        }

    }
    

    ExitThread(0);
    return(0);
}

DWORD WINAPI re_gravity(LPVOID param)
{
   
    if (KeyBuffer[VK_SHIFT] == TRUE)
    {
        
        if (false == IntersectRect(&is, &x, &y_2))
        {
            if (z == 0)
            {
                z += 5;
            }
            else if (z <= 10)
            {
                z += 1;

            }
            x.top -= z;
            x.bottom -= z;
            //InvalidateRect(hWnd, NULL, true);
        }
        else if (true == IntersectRect(&is, &x, &y_2))
        {
            
             z = 0;
            p_jump = FALSE;
        }

        if (KeyBuffer[VK_SPACE])
        {
            if (p_jump == FALSE)
            {
                x.top += 4;
                x.bottom += 4;
                z = -16;
                p_jump = TRUE;
            }
        }
    }
    
    ExitThread(0);
    return(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:

            KeyBuffer[wParam] = TRUE;

            break;

        case VK_RIGHT:

            KeyBuffer[wParam] = TRUE;

            break;

        case VK_SPACE:

            KeyBuffer[wParam] = TRUE;

            break;

        case VK_SHIFT:

            if (KeyBuffer[wParam] == FALSE)
            {
                KeyBuffer[wParam] = TRUE;
                break;
            }
            if (KeyBuffer[wParam] == TRUE)
            {
                KeyBuffer[wParam] = FALSE;
                break;
            }

            break;
        }
    }
    break;

    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_LEFT:

            KeyBuffer[wParam] = FALSE;

            break;

        case VK_RIGHT:

            KeyBuffer[wParam] = FALSE;

            break;

        case VK_SPACE:

            KeyBuffer[wParam] = FALSE;

            break;

        
        }
        break;

    case WM_TIMER:
    {

        if (1 == wParam)
        {
            CreateThread(NULL, 0, gravity, (LPVOID)lParam, 0, NULL);
            CreateThread(NULL, 0, re_gravity, (LPVOID)lParam, 0, NULL);
            CreateThread(NULL, 0, moving, (LPVOID)lParam, 0, NULL);
        }
        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_CREATE:
    {
        g_timer = 1;
        SetTimer(hWnd, 1, g_timer, NULL);

        KeyBuffer[VK_SHIFT] = FALSE;
        p_jump = FALSE;

        platform[1].pf.left = 100;
        platform[1].pf.top = 300;
        platform[1].pf.right = 300;
        platform[1].pf.bottom = 320;

        platform[2].pf.left = 100;
        platform[2].pf.top = 300;
        platform[2].pf.right = 300;
        platform[3].pf.bottom = 320;

        y.left = 10;
        y.top = 400;
        y.right = 500;
        y.bottom = 500;

        y_2.left = 10;
        y_2.top = 0;
        y_2.right = 500;
        y_2.bottom = 100;

        x.left = 10;
        x.top = 200;
        x.right = 30;
        x.bottom = 230;



    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        static HDC hdc, MemDC, tmpDC;
        static HBITMAP BackBit, oldBackBit;
        static RECT bufferRT;
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &bufferRT);
        MemDC = CreateCompatibleDC(hdc);
        BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
        PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;
        
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        Rectangle(hdc, y.left, y.top, y.right, y.bottom);
        Rectangle(hdc, y_2.left, y_2.top, y_2.right, y_2.bottom);
        
        Rectangle(hdc, platform->pf.left, platform->pf.top, platform->pf.right, platform->pf.bottom);

        Rectangle(hdc, x.left, x.top, x.right, x.bottom);

        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;
        
        GetClientRect(hWnd, &bufferRT);
        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
        SelectObject(MemDC, oldBackBit);
        DeleteObject(BackBit);
        DeleteDC(MemDC);
        EndPaint(hWnd, &ps);
        break;
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    }
}
// 정보 대화 상자의 메시지 처리기입니다.
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
