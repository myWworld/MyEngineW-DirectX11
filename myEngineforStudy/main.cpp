// myEngineforStudy.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "myEngineforStudy.h"
#include "..//MyEngine_Source/MEApplication.h"
#include "..//MyEngine_W/MELoadScene.h"
#include "..//MyEngine_Source//MEResources.h"

#include "../MyEngine_Source/METexture.h"
#include "../MyEngine_W/METoolScene.h"
#include "../MyEngine_Source/MESceneManager.h"



ME::Application application;

ULONG_PTR gpToken;
Gdiplus::GdiplusStartupInput gpsi;


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass (HINSTANCE hInstance, const wchar_t* name, WNDPROC proc);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndTileProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(251);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYENGINEFORSTUDY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance , szWindowClass, WndProc);
    MyRegisterClass(hInstance, L"TILEWINDOW", WndTileProc);


    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYENGINEFORSTUDY));

    MSG msg;

    ME::LoadScenes();

    while (true)
    {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {

            //메시지가 없을 경우
              //게임 로직이 들어가면 된다.
          
            application.Run();
        }

    }

    // 기본 메시지 루프입니다:
   // while (GetMessage(&msg, nullptr, 0, 0))
   // {
      //  if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
     //   {
  //          TranslateMessage(&msg);
///DispatchMessage(&msg);
  //      }
   // }
    RemoveFontResourceEx(L"..\\Resources\\jejufont.tff", FR_PRIVATE, 0);

   
    application.Release();

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance,const wchar_t * name,  WNDPROC proc)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = proc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYENGINEFORSTUDY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYENGINEFORSTUDY);
    wcex.lpszClassName  = name ;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   const UINT width = 900;
   const UINT height = 600;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, width,height, nullptr, nullptr, hInstance, nullptr);



 

   if (!hWnd)
   {
      return FALSE;
   }


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);



   application.Initialize(hWnd, width, height);

   //Tile 윈도우 크기 조정

   ME::Scene* activeScene = ME::SceneManager::GetActiveScene();

   std::wstring name = activeScene->GetName();

// if (name == L"ToolScene")
// {
//     HWND ToolHwnd = CreateWindowW(L"TILEWINDOW", L"TileWindow", WS_OVERLAPPEDWINDOW,
//         0, 0, width, height, nullptr, nullptr, hInstance, nullptr);
//
//     ME::graphics::Texture* texture = ME::Resources::Find<ME::graphics::Texture>(L"SPRINGFLOOR");
//
//     RECT rect = { 0,0,texture->GetWidth(),texture->GetHeight() };
//
//
//     UINT toolWidth = rect.right - rect.left;
//     UINT toolHeight = rect.bottom - rect.top;
//
//     AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
//
//
//
//     SetWindowPos(ToolHwnd, nullptr, width, 0, toolWidth, toolHeight, 0);
//
//     ShowWindow(ToolHwnd, true);
//     UpdateWindow(ToolHwnd);
// }


   return TRUE;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            //DC란 화면에 출력에 필요한 모든 정보를 가지는 데이터 구조체
            //GDI모듈에 의해서 관리된다.
            //어떤 폰트를 사용할 건가, 어떤 선의 굵기를 정해줄건가 어떤 색상으로 그려줄껀가
            //화면 출력에 필요한 모든 경우는 WINAPI에서는 DC를 통해서 작업을 진행할 수 있다,

         

            EndPaint(hWnd, &ps);
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
