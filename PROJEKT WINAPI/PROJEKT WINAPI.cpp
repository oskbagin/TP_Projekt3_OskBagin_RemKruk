// PROJEKT WINAPI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PROJEKT WINAPI.h"
#include "Bloczek.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int positionX=450;
int positionY=155;
int ktory=0; 

Bloczek klocuszki[5] = { Bloczek("Kolko", 272, 495), Bloczek("Trojkat", 400, 495), Bloczek("Kolko", 550, 495),
					Bloczek("Kwadrat", 350, 495), Bloczek("Trojkat", 488, 495)};

void rysujKlocuszki(HDC hdc, Bloczek tab[])
{
	Graphics graphics(hdc);
	
	Pen      pen(Color(255, 0, 0, 255),2);

	for(int i=0; i<5; i++){
		// Rysowanie trójk¹ta z grafiki - trzeba wykminiæ, jak zrobiæ przezroczyste poza trójk¹tem
		if(tab[i].getKsztalt() == "Trojkat" && !tab[i].getZlapany()){ 
			/*Image image(L"grafika/trojkat.jpg");
			graphics.DrawImage(&image, tab[i].getX(), tab[i].getY()-65); 		*/
			SolidBrush brush(Color(255, 255, 0, 0));
			Point punkty[3]={ Point(tab[i].getX(), tab[i].getY()), Point(tab[i].getX()+30, tab[i].getY()),
															Point(tab[i].getX()+15, tab[i].getY()-30)};
			graphics.FillPolygon(&brush, punkty, 3);
		}
		// Rysowanie kwadratu
		else if(tab[i].getKsztalt() == "Kwadrat"){
			int x=tab[i].getX(), y=tab[i].getY()-30;
			Rect rect(x, y, 30,30);
			SolidBrush brush(Color(255, 0, 0, 255));

			graphics.FillRectangle(&brush, rect);
		}
		// Rysowanie kó³eczka
		else if(tab[i].getKsztalt() == "Kolko"){
			SolidBrush brush(Color(255, 221, 255, 5));
			graphics.FillEllipse(&brush, tab[i].getX(), tab[i].getY()-28, 30, 30);
		}

			
	}
}

void PoruszajDzwig( HDC hdc, int i, int k)
{
	  Graphics graphics(hdc);
      Pen      pen(Color(255, 100, 100, 100),2);
   	  Image image(L"grafika/dzwig_2.jpg");

	  graphics.DrawImage(&image, 10, 10);
	  rysujKlocuszki(hdc, klocuszki);

	  if(!klocuszki[ktory].getZlapany())
	  {
		  graphics.DrawLine(&pen, positionX, positionY, positionX-10, positionY+45);
		  graphics.DrawLine(&pen, positionX, positionY, positionX+10, positionY+45);
		  graphics.DrawLine(&pen, positionX, 135, positionX, positionY);
	  }
	  else{
		  SolidBrush brush(Color(255, 255, 0, 0));
		  Point punkty[3]={ Point(positionX-15, positionY+30), Point(positionX+15, positionY+30),
															Point(positionX, positionY)};

		  graphics.DrawLine(&pen, positionX, 135, positionX, positionY);
		  graphics.FillPolygon(&brush, punkty, 3);
		  //graphics.DrawImage(&image, positionX-31, positionY-20);
	  }
	  positionX+=i;
	  positionY+=k;
}

// MAIN
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	
	//Start GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJEKTWINAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJEKTWINAPI));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJEKTWINAPI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJEKTWINAPI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1500, 1500, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			wchar_t waCoord[20];
			wsprintf(waCoord, _T(" (%i, %i) "), iPosX, iPosY);
			::MessageBox(hWnd, waCoord, _T("LMB"), MB_OK);
			break;
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		break;
	case WM_PAINT:{
		// TODO: Add any drawing code here...
		hdc = BeginPaint(hWnd, &ps);
		PoruszajDzwig(hdc, 0, 0);
		EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
			//	RysujDzwig(hdc);
			//	EndPaint(hWnd, &ps);
			break;
	case WM_KEYDOWN:
{
    switch (wParam) {
        case VK_RIGHT:
        {
			if(positionX < 660){
				InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,5,0);
				EndPaint(hWnd, &ps);
			}
            break;
        }
		case VK_SPACE:
		{
			for(ktory=0; ktory<5; ktory++){
				if(abs(klocuszki[ktory].getX() - positionX+5) < 15 && 
					positionY > 400 && klocuszki[ktory].getKsztalt() == "Trojkat" || 
					klocuszki[ktory].getZlapany()){
						if(klocuszki[ktory].getZlapany())		klocuszki[ktory].zmienX(positionX-15);

						klocuszki[ktory].zmienZlapany(!klocuszki[ktory].getZlapany());
						::MessageBox(hWnd, _T("ZLAPALEM!"), _T("LMB"), MB_OK);
					break;
				}
			}
			break;
		}
		case VK_END: // instantowe opuszczenie haka na dó³
		{
			positionY = 449;
			InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,5,0);
				EndPaint(hWnd, &ps);
				break;
		}
		case VK_HOME: // instantowe wci¹gniêcie haka na wysokoœæ ramienia
		{
			positionY=155;
			InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,5,0);
				EndPaint(hWnd, &ps);
				break;
		}
		case VK_LEFT:
        {
			if(positionX > 200){
				InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,-5,0);
				EndPaint(hWnd, &ps);
			}
            break;
        }
		//case VK_
		case VK_DOWN:
        {
			if(positionY < 450){
				InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,0,5);
				EndPaint(hWnd, &ps);
			}
            break;
        }
		case VK_UP:
        {
			if(positionY>155){
				InvalidateRect(hWnd, NULL, FALSE);
				hdc = BeginPaint(hWnd, &ps);
				PoruszajDzwig(hdc,0,-5);
				EndPaint(hWnd, &ps);
			}
			break;
        }
    }
    break;
}


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
