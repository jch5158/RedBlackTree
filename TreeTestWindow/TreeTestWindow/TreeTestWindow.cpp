﻿// TreeTestWindow.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "stdafx.h"
#include "CBSearchTree.h"
#include "TreeTestWindow.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE           hInst;                                // 현재 인스턴스입니다.
WCHAR               szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR               szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND                hWnd;
HDC                 hdc;

// 사각 영역을 정의하여 영역안에 문자열을 출력할 수 있다.
RECT                rect;


// 페인트 구조체입니다.
PAINTSTRUCT         ps;


CBSearchTree searchTree;



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//=========================================================================
// hInstance  프로그램의 인스턴스 핸들
// 바로 앞에 실행된 현재 프로그램의 인스턴스 거의 NULL 신경쓰지 않아도 됨
// 커맨드 입력 argv 
// 프로그램이 실행될 형태이며 최소화, 보통 모양등의 전달 InitInstance() 함수에 전달
//=========================================================================
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
    LoadStringW(hInstance, IDC_TREETESTWINDOW, szWindowClass, MAX_LOADSTRING);
 
    // 윈도우 클래스를 정의
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TREETESTWINDOW));

    MSG msg;



    
   
    // 기본 메시지 루프입니다:
    // GetMessage는 시스템이 유지하는 메시지 큐에서 메시지를 읽어들인다. 읽어들인 메시지는 첫번째 인수가 지정하는 MSG 구조체에 저장된다. 
    // 이 함수는 읽어들인 메시지가 프로그램을 종료하라는 WM_QUIT일 경우 False를 리턴하며 그 외의 메시지이면 True를 리턴한다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            // 키보드 입력 메시지를 가공하여 프로그램에서 쉽게 쓸 수 있도록 해 준다. 
            // 전달된 메시지가 WM_KEYDOWN인지와 눌려진 키가 문자키인지 검사해 보고 조건이 맞을 경우 WM_CHAR 메시지를 만들어 메시지 큐에 덧붙이는 역할을 한다
            // 물론 문자 입력이 아닐 경우는 아무 일도 하지 않으며 이 메시지는 DispatchMessage 함수에 의해 WndProc으로 보내진다.
            TranslateMessage(&msg);

            // 시스템 메시지 큐에서 꺼낸 메시지를 프로그램의 메시지 처리 함수(WndProc)로 전달한다. 
            // 결국 메시지 루프가 하는 일이란 메시지 큐에서 메시지를 꺼내 메시지 처리 함수로 보내주는 것 뿐이다.
            // 메시지는 시스템의 변화에 대한 정보이며 MSG라는 구조체에 보관된다. 
            DispatchMessage(&msg);

        }
    }
    // 메시지 루프가 종료되면 프로그램은 마지막으로 Message.wParam을 리턴하고 종료한다. 이 값은 WM_QUIT 메시지로부터 전달된 탈출 코드(exit code)이다. 
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

    // 윈도우가 어떤 형태를 가질 것인가를 지정할 수 있다.
    // 윈도우의 수직, 수평이 바뀔 경우 윈도우를 다시 그린다는 뜻이다.
    // CS_DBLCLKS 은 마우스의 더블 클릭을 지원하기 위함이다.
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    // 윈도우 메시지 처리 함수를 지정한다.
    wcex.lpfnWndProc    = WndProc;

    // 일종의 예약 영역이다. 윈도우즈가 내부적으로 사용하며 아주 특수한 목적에 사용되는 여분의 공간이다.
    // 예약 영역을 사용하지 않을 경우는 0으로 지정한다.
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    
    // 이 윈도우 클래스를 사용하는 프로그램의 번호이다.
    wcex.hInstance      = hInstance;

    // 커서와 프로그램 아이콘을 지정한다.
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TREETESTWINDOW)); 

    // 1. 윈도우즈가 제공하는 디폴트 커서를 사용하려면 이 인수를 NULL로 지정하면 된다.
    // 2. 커서 스타일
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);

    // 윈도우의 배경 색상을 채색할 브러시를 지정한다.
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    
    // 이 프로그램이 사용할 메뉴를 지정한다.
    // 리소스 에디터에 의해 별도로 만들어진 후 링크시에 같이 합쳐진다.
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TREETESTWINDOW);
    
    // 윈도우 클래스의 이름을 정의한다.
    wcex.lpszClassName  = szWindowClass;

    // 작은 아이콘에 대한 핸들입니다.
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // RegisterClass 함수의 인수로 WndClass 구조체의 번지를 넘겨주면 된다.이런 이런 특성을 가진 윈도우를 앞으로 사용하겠다는 등록 과정이다.
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

   //  1. 생성하고자 하는 윈도우의 클래스를 지정하는 문자열이다.
   //  2. 윈도우의 타이틀 바에 나타날 문자열이다
   //  3. 윈도우가 경계선을 가질 것인가, 타이틀 바를 가질 것인가 또는 스크롤 바의 유무 등등을 세세하게 지정해 줄 수 있다.  
   //     WS_OVERLAPPEDWINDOW를 사용하면 가장 무난한 윈도우 설정 | (or) 연산으로 스크롤이나 윈도우창 속성을 추가할 수 있다.
   //  4~7. 생성좌표, 윈도우 창 크기
   //  8. 부모 윈도우가 있을 경우 부모 윈도우의 핸들을 지정해 준다. MDI 프로그램이나 팝업 윈도우는 윈도우끼리
   //     수직적인 상하관계를 가져 부자(parent-child) 관계가 성립되는데 이 관계를 지정해 주는 인수이다. 
   //  9. WndClass에서 지정한 메뉴 대신 다른 메뉴를 사용하려면 이 인수에 원하는 메뉴 핸들을 주면 된다
   // 10. 윈도우를 만드는 주체, 즉 프로그램의 핸들을 지정한다. WinMain의 인수로 전달된 hInstance를 대입해 주면 된다.
   // 11. CREATESTRUCT라는 구조체의 번지이며 특수한 목적에 사용된다. 보통은 NULL값을 사용한다.
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, 1400, 3000, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   // hWnd 인수는 화면으로 출력하고자 하는 윈도우의 핸들이며 CreateWindow 함수가 리턴한 핸들을 그대로 넘겨주면 된다.
   // SW_HIDE 이거 인자로 넘기면 프로세스 숨겨짐...못된짓 할 때 사용..
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static int x;
    static int y;
    
    
    // insert 메뉴를 선택했을 때
    static bool insertFlag = false;

    // delete menu를 선택했을 때
    static bool deleteFlag = false;

    // 매뉴를 선택했을 때
    static bool menuSelectFlag = true;

    static char str[256];

    static HBRUSH oldBrush, redBrush, blackBrush;

    // 문자열의 길이를 얻기 위한 변수
    int strLen;

    switch (message)
    {
    case WM_CREATE:

        // 가제로 메시지를 발생시키는 함수입니다.
        // 두 번째는 메시지 이름
        // 세 번째, 네 번째는 wParam,lParam입니다.
        //SendMessage(hWnd, WM_LBUTTONDOWN, 30, 50);

        redBrush = CreateSolidBrush(RGB(255, 0, 0));

        blackBrush = CreateSolidBrush(RGB(0, 0, 0));

        break;
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

     // 더블 클릭에 대한 메시지이다.
    //case WM_LBUTTONDBLCLK:

    //    InvalidateRect(hWnd, nullptr, true);

    //    break;


    case WM_CHAR:
  
        if (menuSelectFlag == true) {
 
            strLen = strlen(str);

            str[strLen] = (char)wParam;

            str[strLen + 1] = '\0';

            if ((atoi(str) == 1))
            {
                insertFlag = true;
                deleteFlag = false;
                menuSelectFlag = false;
                memset(str, '\0', strlen(str));
            }
            else if (atoi(str) == 2)
            {
                insertFlag = false;
                deleteFlag = true;
                menuSelectFlag = false;
                memset(str, '\0', strlen(str));
            }
            else if (atoi(str) == 3)
            {

                menuSelectFlag = true;
                insertFlag = false;
                deleteFlag = false;

                memset(str, '\0', sizeof(str));

                searchTree.ReleaseTree();
            }
            else
            {
                insertFlag = false;
                deleteFlag = false;
                menuSelectFlag = true;
                memset(str, '\0', strlen(str));
            }
        } 
        else
        {
            strLen = strlen(str);

            str[strLen] = (char)wParam;

            str[strLen + 1] = '\0';
        }

        if (wParam == VK_RETURN)
        {
            
            if (insertFlag == true)
            {
                searchTree.InsertNode(atoi(str));
            }
            else if (deleteFlag == true)
            {
                searchTree.DeleteNode(atoi(str));
            }

            menuSelectFlag = true;
            insertFlag = false;
            deleteFlag = false;
            
            memset(str, '\0', sizeof(str));
        }

        //==============================================================================================================
        // 이 함수는 윈도우의 작업 영역을 무효화시켜 윈도우즈로 하여금 WM_PAINT 메시지를 해당 윈도우로 보내도록 한다.
        // 1. 첫 번째 인수는 무효화 대상이되는 윈도우 핸들이다.
        // 2. 두 번째 인수 lpRect는 무효화의 대상이 되는 사각 영역을 써 주되 이 값이 NULL이면 윈도우의 전 영역이 무효화된다.
        // 3. 세 번째 인수 true일 경우 배경을 지우고 WM_FAINT를 호출할지 false일 경우 배경을 지우지 WM_FAINT를 호출할지를 정한다.
        //==============================================================================================================
        InvalidateRect(hWnd, nullptr, true);

        break;

    // WM_PAINT 메시지는 "너의 작업 영역이 일부 지워졌으니까 빨리 복구해"라는 뜻이다. 
    case WM_PAINT:
        {    
            // BeginPaint는 WM_PAINT 메시지내에서 그림 그리기 준비를 하는 좀 더 전문적인 함수이다.
            // BeginPaint 함수는 윈도우 핸들 외에도 페인트 정보 구조체를 인수로 요구하며 이 구조체는 그림 그리기에 필요한 정보를 담는다.
            hdc = BeginPaint(hWnd, &ps);
 
            //oldBrush = (HBRUSH)SelectObject(hdc, redBrush);
 
            //SelectObject(hdc,oldBrush);

            searchTree.Print(hdc);

            char menu[] = "1. Input     2. delete     3. All Delete";
            TextOutA(hdc, 50, 700, menu, strlen(menu));

            if (insertFlag == true)
            {
                WCHAR menuValue[] = L"입력 : ";
                TextOut(hdc, 50, 730, menuValue, wcslen(menuValue));
            }
            else if (deleteFlag == true)
            {
                WCHAR menuValue[] = L"삭제 값 입력 : ";
                TextOut(hdc, 50, 730, menuValue, wcslen(menuValue));
            }

            if (menuSelectFlag == false)
            {
                TextOutA(hdc, 200, 730, str, strlen(str));
            }

            EndPaint(hWnd, &ps); 
        }
        break;
    case WM_DESTROY:

        DeleteObject(redBrush);

        DeleteObject(blackBrush);

        // PostQuitMessage 함수를 호출하여 WM_QUIT 메시지를 보낸다. WM_QUIT 메시지가 입력되면 
        // 메시지 루프의 GetMessage 함수 리턴값이 False가 되어 프로그램이 종료된다.
        PostQuitMessage(0);
        break;
    default:
        //  윈도우의 이동이나 크기변경 따위의 처리는 직접 해 줄 필요없이 DefWindowProc으로 넘겨주기만 하면 된다.
        // 또한 DefWindowProc 함수가 메시지를 처리했을 경우 이 함수가 리턴한 값을 WndProc 함수가 다시 리턴해 주어야 한다.
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
