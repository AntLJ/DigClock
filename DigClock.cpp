// DigClock.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DigClock.h"

#define MAX_LOADSTRING 100
#define ID_TIMER 1 

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
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

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIGCLOCK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIGCLOCK));

    MSG msg;

    // メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIGCLOCK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIGCLOCK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

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

void DisplayDigit(HDC hdc, int iNumber)
{
	static BOOL fSevenSegment[10][7] = {
	1, 1, 1, 0, 1, 1, 1, // 0
	0, 0, 1, 0, 0, 1, 0, // 1
	1, 0, 1, 1, 1, 0, 1, // 2
	1, 0, 1, 1, 0, 1, 1, // 3
	0, 1, 1, 1, 0, 1, 0, // 4
	1, 1, 0, 1, 0, 1, 1, // 5
	1, 1, 0, 1, 1, 1, 1, // 6
	1, 0, 1, 0, 0, 1, 0, // 7
	1, 1, 1, 1, 1, 1, 1, // 8
	1, 1, 1, 1, 0, 1, 1 }; // 9

	static POINT ptSegment[7][6] = {
	7, 6, 11, 2, 31, 2, 35, 6, 31, 10, 11, 10,
	6, 7, 10, 11, 10, 31, 6, 35, 2, 31, 2, 11,
	36, 7, 40, 11, 40, 31, 36, 35, 32, 31, 32, 11,
	7 , 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,
	6 , 37, 10, 41, 10, 61, 6, 65, 2, 61, 2, 41,
	36, 37, 40, 41, 40, 61, 36, 65, 32, 61, 32, 41,
	7 , 66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70 };
	int iSeg;
	for (iSeg = 0; iSeg < 7; iSeg++) // 7 is the number of digital numbers' displaying segements, the number 8 displays 7 segements, other numbers display less than 7 segements.
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6); // 6 means the count of the polygon's vertexes
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit(hdc, iNumber / 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
	DisplayDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}
void DisplayColon(HDC hdc)
{
	POINT ptColon[2][4] = { 2, 21, 6, 17, 10, 21, 6,
	25, 2, 51, 6, 47, 10, 51, 6, 55 };
	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);
	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}
void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (f24Hour)
		DisplayTwoDigits(hdc, st.wHour, fSuppress);
	else
		DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wMinute, FALSE);
	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wSecond, FALSE);
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL f24Hour, fSuppress;
	static HBRUSH hBrushRed;
	static int cxClient, cyClient;
    switch (message)
    {
	case WM_CREATE:
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		SetTimer(hWnd, ID_TIMER, 1000, NULL);// fall through 
	case WM_SETTINGCHANGE:
		TCHAR szBuffer[2];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		f24Hour = (szBuffer[0] == '1');
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
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

			SetMapMode(hdc, MM_ISOTROPIC);
			SetWindowExtEx(hdc, 276, 72, NULL);
			SetViewportExtEx(hdc, cxClient, cyClient, NULL);
			SetWindowOrgEx(hdc, 138, 36, NULL);
			SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
			SelectObject(hdc, GetStockObject(NULL_PEN));
			SelectObject(hdc, hBrushRed);
			DisplayTime(hdc, f24Hour, fSuppress);

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

// バージョン情報ボックスのメッセージ ハンドラーです。
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
