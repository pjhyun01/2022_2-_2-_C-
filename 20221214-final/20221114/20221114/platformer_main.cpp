// 20221114.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "20221114.h"
#include <thread>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

enum class SceneType : size_t;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20221114));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	RECT rt = { 0,0,1200, 900 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,rt.right - rt.left, rt.bottom - rt.top, nullptr, nullptr, hInstance, nullptr);

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

//구조체 및 변수들 선언

int g_timer;
int j_power = 0;
int map = 0;
//size_t scene_count[(size_t)SceneType::Scene_Count];

BOOL p_isjump;
BOOL p_canDjump;
BOOL KeyBuffer[256];
BOOL Grav_direction;
BOOL p_iscollision;
BOOL g_check;
BOOL p_isdeath;
BOOL p_canrev;


struct Object
{
	int x;
	int y;
	int height;
	int width;
};

struct Spike
{
	int x;
	int y;
	int line;
};

struct RevSpike
{
	int x;
	int y;
	int line;
};

POINT SpikePoints[3];
Object finish;
Object player;
Object goal;
Object item_DoubleJ;
Spike spike;
RevSpike revspike;

enum class SceneType : size_t
{
	kStart = 0,
	kJump,
	kRevGrav,
	kTestLevel,
	kTestLevel2,
	kTestLevel3,
	kEnd
};

vector<Object> objects;
vector<Spike> spikes;
vector<RevSpike> revspikes;

vector<vector<Object>> maps_objects((size_t)SceneType::kEnd);
vector<vector<Spike>> maps_spikes((size_t)SceneType::kEnd);
vector<vector<RevSpike>> maps_revspikes((size_t)SceneType::kEnd);


HANDLE handle;

//maps_objects[(size_t)SceneType::kDefault];


bool IsCollision(Object a, Object b);
bool IsTouchSpike(Spike a, Object b);
bool IsTouchRevSpike(RevSpike a, Object b);

void Create(Object object);
void CreateSpike(Spike spike);
void CreateRevSpike(RevSpike revspike);
void DrawObject(HDC hdc, Object object);
void DrawSpike(HDC hdc, Spike spike);
void DrawRevSpike(HDC hdc, RevSpike spike);

//쓰레드 만들어둔 곳


DWORD WINAPI moving(LPVOID param)
{
	if (KeyBuffer[VK_LEFT])
	{
		player.x = clamp(player.x - 4, 0, 1170);
	}

	if (KeyBuffer[VK_RIGHT])
	{
		player.x = clamp(player.x + 4, 0, 1170);
	}

	ExitThread(0);
	return(0);
}

DWORD WINAPI JUMP(LPVOID param)
{
	if (KeyBuffer[VK_SPACE])
	{
		if (p_isjump == FALSE)
		{
			if (false == Grav_direction)
			{
				player.y -= 5;
				j_power = -16;
			}
			if (true == Grav_direction)
			{
				player.y += 5;
				j_power = 16;
			}
			p_isjump = TRUE;
		}
	}

	ExitThread(0);
	return(0);
}


DWORD WINAPI GRAVITY(LPVOID param)
{
	if (false == p_iscollision)
	{
		if (false == Grav_direction)
		{
			if (j_power <= 10)
			{
				j_power += 1;
			}
		}

		if (true == Grav_direction)
		{
			if (j_power >= -10)
			{
				j_power -= 1;
			}
		}
	}

	else if (true == p_iscollision)
	{
		j_power = 0;
		
		if (false == Grav_direction)
		{
			j_power += 10;
		}
		if (true == Grav_direction)
		{
			j_power -= 10;
		}
	}

	player.y += j_power;

	ExitThread(0);
	return(0);
}

DWORD WINAPI ISCOLLISION(LPVOID param)
{
	while (1)
	{
		for (size_t i = 0; i < maps_objects[map].size(); i++)
		{
			if (IsCollision(player, maps_objects[map][i]))
			{
				if (player.y + player.height / 2 >= maps_objects[map][i].y - maps_objects[map][i].height / 2 && player.y - player.height / 2 <= maps_objects[map][i].y + maps_objects[map][i].height / 2)
				{
					if (player.y > maps_objects[map][i].y)
					{
						player.y += (maps_objects[map][i].y + maps_objects[map][i].height / 2) - (player.y - player.height / 2);
					}
					else if (player.y < maps_objects[map][i].y)
					{
						player.y -= (player.y + player.height / 2) - (maps_objects[map][i].y - maps_objects[map][i].height / 2);
					}
				}
				if (p_isjump == true)
				{
					p_isjump = false;
				}
				if (p_canrev == false)
				{
					p_canrev = true;
				}
				p_iscollision = true;

				SuspendThread(handle);
			}
			else
			{
				ResumeThread(handle);
				p_iscollision = false;
			}
		}
		
		for (size_t i = 0; i < maps_spikes[map].size(); i++)
		{
			if (IsTouchSpike(maps_spikes[map][i], player))
			{
				p_isdeath = true;
			}
		}
		
		for (size_t i = 0; i < maps_revspikes[map].size(); i++)
		{
			if (IsTouchRevSpike(maps_revspikes[map][i], player))
			{
				p_isdeath = true;
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

	case WM_CREATE:
	{
		g_timer = 1;
		SetTimer(hWnd, 1, g_timer, NULL);

		p_isjump = FALSE;
		p_iscollision = FALSE;
		p_isdeath = FALSE;
		p_canDjump = FALSE;
		Grav_direction = FALSE;
		p_canrev = TRUE;

		player.x = 30;
		player.y = 600;
		player.width = 20;
		player.height = 30;

		finish.x = 1100;
		finish.y = 615;
		finish.width = 30;
		finish.height = 30;
		
		//시작맵
		map = 5;

		maps_objects[0].push_back({600,750,200,1200});

		maps_objects[1].push_back({600,750,200,1200});
		maps_spikes[1].push_back({600, 635,50});

		maps_objects[2].push_back({ 50,750,200,400 });
		maps_objects[2].push_back({ 1200, 750, 200, 400 });
		maps_objects[2].push_back({ 650, 0 ,100,1000 });

		//4번째 맵 오브젝트
		maps_objects[3].push_back({ 600,750,200,1200 });
		maps_objects[3].push_back({ 600,0,200,1200 });
		//4번째 맵 정방향 가시
		maps_spikes[3].push_back({ 250, 635,50 });
		maps_spikes[3].push_back({ 300, 635,50 });
		maps_spikes[3].push_back({ 350, 635,50 });
		maps_spikes[3].push_back({ 550, 635,50 });
		maps_spikes[3].push_back({ 600, 635,50 });
		maps_spikes[3].push_back({ 650, 635,50 });
		maps_spikes[3].push_back({ 850, 635,50 });
		maps_spikes[3].push_back({ 900, 635,50 });
		maps_spikes[3].push_back({ 950, 635,50 });
		//4번째 맵 역방향 가시
		maps_revspikes[3].push_back({ 400,115,50 });
		maps_revspikes[3].push_back({ 450,115,50 });
		maps_revspikes[3].push_back({ 500,115,50 });
		maps_revspikes[3].push_back({ 700,115,50 });
		maps_revspikes[3].push_back({ 750,115,50 });
		maps_revspikes[3].push_back({ 800,115,50 });

		//5번째 맵 오브젝트
		maps_objects[4].push_back({ 100, 750,200,200 });
		maps_objects[4].push_back({ 100,30,20,200 });
		maps_objects[4].push_back({ 300,130,20,200 });
		maps_objects[4].push_back({ 500,230,20,200 });
		maps_objects[4].push_back({ 700,330,20,200 });
		maps_objects[4].push_back({ 900,430,20,200 });
		maps_objects[4].push_back({ 1100,530,20,200 });

		//6번째 맵 오브젝트
		maps_objects[5].push_back({ 100, 750,200,200 });
		//발판들
		maps_objects[5].push_back({ 100,30,20,200 });
		maps_objects[5].push_back({ 300,130,20,200 });
		maps_objects[5].push_back({ 500,230,20,200 });
		maps_objects[5].push_back({ 700,330,20,200 });
		maps_objects[5].push_back({ 900,430,20,200 });
		maps_objects[5].push_back({ 1100,530,20,200 });
		//6번쨰 맵 가시
		maps_spikes[5].push_back({ 500,205,50 });
		maps_spikes[5].push_back({ 450,205,50 });
		maps_spikes[5].push_back({ 550,205,50 });
		maps_spikes[5].push_back({ 850,405,50 });
		maps_spikes[5].push_back({ 900,405,50 });
		maps_spikes[5].push_back({ 950,405,50 });
		//6번째 맵 역방향 가시
		maps_revspikes[5].push_back({ 250,155,50 });
		maps_revspikes[5].push_back({ 300,155,50 });
		maps_revspikes[5].push_back({ 350,155,50 });
		maps_revspikes[5].push_back({ 650,355,50 });
		maps_revspikes[5].push_back({ 700,355,50 });
		maps_revspikes[5].push_back({ 750,355,50 });

		CreateThread(NULL, 0, ISCOLLISION, (LPVOID)lParam, 0, NULL);
		
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
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

			if (Grav_direction == FALSE && p_canrev == TRUE)
			{
				player.y -= 5;
				Grav_direction = TRUE;
				p_canrev = FALSE;
			}
			else if (Grav_direction == TRUE && p_canrev == TRUE)
			{
				player.y += 5;
				Grav_direction = FALSE;
				p_canrev = FALSE;
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

	}

	case WM_TIMER:
	{
		if (1 == wParam)
		{
			handle = CreateThread(NULL, 0, GRAVITY, (LPVOID)lParam, 0, NULL);
			CreateThread(NULL, 0, JUMP, (LPVOID)lParam, 0, NULL);
			CreateThread(NULL, 0, moving, (LPVOID)lParam, 0, NULL);

			if (player.y + player.height / 2 >= 750 || player.y - player.height / 2 <= 0)
			{
				p_isdeath = true;
			}

			if (p_isdeath == true)
			{
				player.x = 30;
				player.y = 600;
				p_isdeath = false;
				j_power = 0;
				Grav_direction = FALSE;
			}

			if (IsCollision(player, finish))
			{
				if (map < (size_t)SceneType::kEnd - 1)
				{
					map += 1;
					player.x = 30;
					player.y = 600;
					j_power = 0;
					Grav_direction = FALSE;
				}
				else
				{
					KillTimer(hWnd, g_timer);
					
					MessageBox(hWnd, L"아마도", L"끝났습니다", MB_OK);
				}
			}

		}

		InvalidateRect(hWnd, NULL, false);
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

		if (map == 0)
		{
			TextOut(hdc, 350, 600, TEXT("좌우 방향키로 이동 가능"), 13);
		}
		else if (map == 1)
		{
			TextOut(hdc, 450, 600, TEXT("'SPACE'를 눌러 점프"), 14);
		}
		else if (map == 2)
		{
			TextOut(hdc, 250, 500, TEXT("'SHIFT'를 눌러 중력 반전"), 17);
		}

		for (size_t i = 0; i < maps_objects[map].size(); i++)
		{
			DrawObject(hdc, maps_objects[map][i]);
		}
		for (size_t i = 0; i < maps_spikes[map].size(); i++)
		{
			DrawSpike(hdc, maps_spikes[map][i]);
		}
		for (size_t i = 0; i < maps_revspikes[map].size(); i++)
		{
			DrawRevSpike(hdc, maps_revspikes[map][i]);
		}

		DrawObject(hdc, player);
		DrawObject(hdc, finish);

		//for (size_t i = 0; i < objects.size(); i++)
		//{
			//DrawObject(hdc, objects[i]);
		//}

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

	case WM_GETMINMAXINFO:
		((LPMINMAXINFO)lParam)->ptMinTrackSize.x = 1200;        // 윈도우 최소 사이즈 x
		((LPMINMAXINFO)lParam)->ptMinTrackSize.y = 750;        // 윈도우 최소 사이즈 y
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = 1200;        // 윈도우 최대 사이즈 x
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = 750;        // 윈도우 최대 사이즈 y
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}

bool IsCollision(Object a, Object b)
{
	if (a.y - a.height / 2 <= b.y + b.height / 2 &&
		a.x + a.width / 2 >= b.x - b.width / 2 &&
		a.x - a.width / 2 <= b.x + b.width / 2 &&
		a.y + a.height / 2 >= b.y - b.height / 2)
	{
		return true;
	}

	return false;
}

bool IsTouchSpike(Spike a, Object b)
{
	if (a.y - a.line / 3 * 2 <= b.y + b.height / 2 &&
		a.x + a.line / 2 >= b.x - b.width / 2 &&
		a.x - a.line / 2 <= b.x + b.width / 2 &&
		a.y + a.line / 3 >= b.y - b.height / 2)
	{
		if (IsCollision({a.x, static_cast<int>(a.y + a.line * (3 * (sqrt(3) / 8))) ,  static_cast<int>(a.line * (sqrt(3) / 4)), a.line}, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y + a.line * (sqrt(3) / 8)) ,  static_cast<int>(a.line * (sqrt(3) / 2)), a.line * (3 / 4) }, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y - a.line * (sqrt(3) / 8)) ,  static_cast<int>(a.line * (sqrt(3) / 4)), a.line / 2 }, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y - a.line * (3 * sqrt(3)) / 8 ),  static_cast<int>(a.line * (sqrt(3) / 4)), a.line / 4 }, b))
		{
			return true;
		}
	}

	return false;
}

bool IsTouchRevSpike(RevSpike a, Object b)
{
	if (a.y - a.line / 3 <= b.y + b.height / 2 &&
		a.x + a.line / 2 >= b.x - b.width / 2 &&
		a.x - a.line / 2 <= b.x + b.width / 2 &&
		a.y + a.line / 3 * 2 >= b.y - b.height / 2)
	{
		if (IsCollision({ a.x, static_cast<int>(a.y + a.line * (3 * (sqrt(3) / 8))) ,  static_cast<int>(a.line * (sqrt(3) / 4)), a.line / 4}, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y + a.line * (sqrt(3) / 8)) ,  static_cast<int>(a.line * (sqrt(3) / 4)), a.line / 2 }, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y - a.line * (sqrt(3) / 8)) ,  static_cast<int>(a.line * (sqrt(3) / 4)), a.line * (3 / 4) }, b))
		{
			return true;
		}

		if (IsCollision({ a.x, static_cast<int>(a.y - a.line * (3 * sqrt(3)) / 8),  static_cast<int>(a.line * (sqrt(3) / 4)), a.line }, b))
		{
			return true;
		}
	}
	return false;
}

void Create(Object object)
{
	objects.push_back(object);
}

void CreateSpike(Spike spike)
{
	spikes.push_back(spike);
}

void CreateRevSpike(RevSpike revspike)
{
	revspikes.push_back(revspike);
}

void DrawObject(HDC hdc, Object object)
{
	Rectangle(hdc, object.x - object.width / 2, object.y - object.height / 2, object.x + object.width / 2, object.y + object.height / 2);
}

void DrawSpike(HDC hdc, Spike spike)
{
	SpikePoints[0].x = spike.x;
	SpikePoints[0].y = spike.y - (spike.line / sqrt(3));

	SpikePoints[1].x = spike.x - (spike.line / 2);
	SpikePoints[1].y = spike.y + spike.line / (2 * sqrt(3));

	SpikePoints[2].x = spike.x + (spike.line / 2);
	SpikePoints[2].y = spike.y + spike.line / (2 * sqrt(3));

	Polygon(hdc, SpikePoints, 3);
}

void DrawRevSpike(HDC hdc, RevSpike spike)
{
	SpikePoints[0].x = spike.x;
	SpikePoints[0].y = spike.y + (spike.line / sqrt(3));

	SpikePoints[1].x = spike.x - (spike.line / 2);
	SpikePoints[1].y = spike.y - spike.line / (2 * sqrt(3));

	SpikePoints[2].x = spike.x + (spike.line / 2);
	SpikePoints[2].y = spike.y - spike.line / (2 * sqrt(3));

	Polygon(hdc, SpikePoints, 3);
}
