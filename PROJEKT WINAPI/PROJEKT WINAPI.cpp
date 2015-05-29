// PROJEKT WINAPI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PROJEKT WINAPI.h"
#include "Bloczek.h"

//defines & consts
#define MAX_LOADSTRING 100
#define TROJKAT 0
#define KOLKO   1
#define KWADRAT 2
#define ID_TIMER 3
#define ID_BUTTON1 4
#define ID_BUTTON2 5
#define ID_BUTTON3 6
const int MaxKlockow=10;
const int MAX_X=660;
const int MIN_X=255;
const int MAX_Y=467;
const int MIN_Y=155;

// Global Variables:
HWND hwndButton;								// przycisk
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int positionX=450;
int positionY=155;
int ktory;
int ileKlockow;
bool trzymaKlocek;
RECT drawArea1 = { 0, 0, 1024, 600 };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

Bloczek klocuszki[MaxKlockow] = { Bloczek(Kolko, 272, 495), Bloczek(Trojkat, 400, 495), Bloczek(Trojkat, 550, 495),
					Bloczek(Kwadrat, 350, 495), Bloczek(Trojkat, 488, 495)};

void rysujKlocuszki(HDC hdc, Bloczek tab[])
{
	Graphics graphics(hdc);
	
	Pen      pen(Color(255, 0, 0, 255),2);

	for(int i=0; i<ileKlockow; i++){
		// Rysowanie trójk¹ta
		if(tab[i].getKsztalt() == TROJKAT && !tab[i].getZlapany()){ 
			SolidBrush brush(Color(255, 255, 0, 0));
			Point punkty[3]={ Point(tab[i].getX(), tab[i].getY()), Point(tab[i].getX()+30, tab[i].getY()),
															Point(tab[i].getX()+15, tab[i].getY()-30)};
			graphics.FillPolygon(&brush, punkty, 3);
		}
		// Rysowanie kwadratu
		else if(tab[i].getKsztalt() == KWADRAT){
			int x=tab[i].getX(), y=tab[i].getY()-30;
			Rect rect(x, y, 30,30);
			SolidBrush brush(Color(255, 0, 0, 255));

			graphics.FillRectangle(&brush, rect);
		}
		// Rysowanie kó³eczka
		else if(tab[i].getKsztalt() == KOLKO){
			SolidBrush brush(Color(255, 221, 255, 5));

			graphics.FillEllipse(&brush, tab[i].getX(), tab[i].getY()-28, 30, 30);
		}	
	}
}

void PoruszajDzwig( HDC hdc, int i, int k, HWND hWnd)
{
	  // DOUBLE BUFFER
	  RECT rcClient;
	  GetClientRect(hWnd, &rcClient);
 
	  HDC hdcMem = CreateCompatibleDC(hdc); // ekran w pamieci
	  const int nMemDC = SaveDC(hdcMem); // zapisanie danych
 
	  HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top); // stworzenie bitmapy
	  SelectObject(hdcMem, hBitmap); //z ekranu w pamieci na bitmape
 
	  Graphics graphics(hdcMem);
	  SolidBrush back(Color(255, 255, 255)); //biale tlo
	  SolidBrush points(Color(255, 0, 0));
 
      //wypelnienie tla
	  graphics.FillRectangle(&back, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); //SM_CXSCREEN - szerokosc ekranu, SM_CYSCREEN - wysokosc ekranu

      Pen      pen(Color(255, 100, 100, 100),2);
   	  Image image(L"grafika/dzwig_2.jpg");

	  graphics.DrawImage(&image, 10, 10);
	  rysujKlocuszki(hdcMem, klocuszki);

	  if(!klocuszki[ktory].getZlapany())
	  {
		  graphics.DrawLine(&pen, positionX, positionY, positionX-5, positionY+5);
		  graphics.DrawLine(&pen, positionX, positionY, positionX+5, positionY+5);
		  graphics.DrawLine(&pen, positionX, 135, positionX, positionY);
	  }
	  else{
		  SolidBrush brush(Color(255, 255, 0, 0));
		  Point punkty[3]={ Point(positionX-15, positionY+25), Point(positionX+15, positionY+25),
															Point(positionX, positionY-5)};
		  graphics.DrawLine(&pen, positionX, 135, positionX, positionY);
		  graphics.FillPolygon(&brush, punkty, 3);
	  }
	  positionX+=i;
	  positionY+=k;

	  //Dalsza czesc double buffera
	  RECT rcClip;
		GetClipBox(hdc, &rcClip); // obszar przycinania
		//kopiowanie z pamieci na nasz ekran
		BitBlt(hdc, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, hdcMem, rcClip.left, rcClip.top, SRCCOPY);
		//kasowanie
		RestoreDC(hdcMem, nMemDC);
		DeleteObject(hBitmap);
		DeleteObject(hdcMem);
}

void spadaj()
{
	int klocekPod=ileKlockow+1;
	for(int i=0; i<ileKlockow; i++){
		if(i==ktory) continue;
		if(abs(15+klocuszki[i].getX()-klocuszki[ktory].getX()) < 10){
			klocekPod=i;
			break;
		}
	}

	static int wywolanie;
		if( MAX_Y-klocuszki[ktory].getY() > 15){
			klocuszki[ktory].zmienY(klocuszki[ktory].getY()+static_cast<int>(0.4*wywolanie*wywolanie)-
								(klocekPod==6?0:klocuszki[klocekPod].getY()));
			wywolanie++;	
		}
		else{
				klocuszki[ktory].zmienY(MAX_Y+28-(klocekPod==6?0:klocuszki[klocekPod].getY()));
				wywolanie=0;
		}
}

bool sprawdzKolizje(int klawisz)
{
	if(!trzymaKlocek) return false;
	for(int i=0; i<ileKlockow; i++){
		if(i==ktory) continue;
		if((klawisz==VK_LEFT && abs(klocuszki[i].getX()-positionX+42)<5)    // kolizja z prawej strony
			|| (klawisz==VK_RIGHT && abs(klocuszki[i].getX()-positionX-15)<5)  // kolizja z lewej strony
			|| (klawisz==VK_SPACE || klawisz==VK_DOWN && abs(klocuszki[i].getX()-positionX+15)<25 && klocuszki[i].getY()-positionY < 57)
			){ return true; }
	}
	return false;
}

//void postawNaWiezy()
//{
//	int i;
//	for( i=0; i<ileKlockow; i++){
//		if(i==ktory) continue;
//		if(abs(positionX-klocuszki[i].getX()+15) < 5){
//			if(positionY>klocuszki[i].getY()+15) < 5)
//		}
//	}
//}

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
   ileKlockow=5;
   HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1280, 768, NULL, NULL, hInstance, NULL);
   /* metro mWT2015 kolo 9:00 */
  // uno.biomed.gda.pl
   hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Kwadrat"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1100, 60,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);          

	hwndButton = CreateWindow(TEXT("button"), TEXT("Trójk¹t"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1100, 120,
		80, 50, hWnd, (HMENU)ID_BUTTON2, hInstance, NULL);          

   hwndButton = CreateWindow(TEXT("button"), TEXT("Kó³eczko"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1100, 180,
		80, 50, hWnd, (HMENU)ID_BUTTON3, hInstance, NULL);          

   if (!hWnd)
   {
      return FALSE;
   }
   SetTimer( hWnd, ID_TIMER, 200, NULL );

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
			wchar_t pozycja[20];
			wsprintf(pozycja, _T(" (%i, %i) "), iPosX, iPosY);
			::MessageBox(hWnd, pozycja, _T("LMB"), MB_OK);
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
	case WM_TIMER:{
		if(!klocuszki[ktory].getZlapany() && klocuszki[ktory].getY()<MAX_Y+25){
			spadaj();
		}
			InvalidateRect(hWnd, &drawArea1, FALSE);
			break;
				  }
	case WM_PAINT:{
		// TODO: Add any drawing code here...
		hdc = BeginPaint(hWnd, &ps);
		PoruszajDzwig(hdc, 0, 0,hWnd);
		EndPaint(hWnd, &ps);
		}
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		KillTimer( hWnd, ID_TIMER );
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		InvalidateRect(hWnd, &drawArea1, FALSE);
		hdc = BeginPaint(hWnd, &ps);

		switch (wParam) {
			case VK_SPACE:
			{
				int i=0;
				for(ktory=0; ktory<ileKlockow; ktory++){
					if(abs(klocuszki[ktory].getX() - positionX+15) < 5 && 
						(abs(klocuszki[ktory].getY()-27 - positionY) < 5) && klocuszki[ktory].getKsztalt() == TROJKAT || 
						klocuszki[ktory].getZlapany()){
							if(klocuszki[ktory].getZlapany()){
 							for( i = 0; i < ileKlockow ; i++)
							{
								if (i==ktory) continue;
								if ( (klocuszki[i].getKsztalt() == TROJKAT) && (abs(klocuszki[i].getX()+25 - positionX) < 25 ) && positionY>410)
								{
									klocuszki[ktory].zmienY(positionY+25);
									klocuszki[ktory].zmienX(positionX-15);
									break;
								}
								else if (positionY<380 ||(klocuszki[i].getKsztalt()==KOLKO && (abs(klocuszki[i].getX() - positionX+5) < 25 )) 
									|| (klocuszki[i].getKsztalt()==KWADRAT && (abs(klocuszki[i].getX() - positionX+5) < 25 )) )
								{
								::MessageBox(hWnd, _T("Nie mozna odstawic elementu"), _T("ERROR"), MB_OK);	
								return 0;
								}
								else
								{
									klocuszki[ktory].zmienX(positionX-15);
									klocuszki[ktory].zmienY(positionY+25);
								}	
							}			
						}
						klocuszki[ktory].zmienZlapany(!klocuszki[ktory].getZlapany());
									
						if(klocuszki[ktory].getZlapany())	::MessageBox(hWnd, _T("ZLAPALEM!"), _T(""), MB_OK);
						else								::MessageBox(hWnd, _T("ODSTAWIONO!"), _T(""), MB_OK);
						trzymaKlocek=klocuszki[ktory].getZlapany();
					break;
				}
			}
			break;
		}
		case VK_END: // instantowe opuszczenie haka na dó³
		{
			positionY = MAX_Y;
			PoruszajDzwig(hdc,0,0,hWnd);
			break;
		}
		case VK_HOME: // instantowe wci¹gniêcie haka na wysokoœæ ramienia
		{
			positionY=MIN_Y;
			PoruszajDzwig(hdc,0,0,hWnd);
			break;
		}
		case VK_LEFT:
        {
			bool kolizja=sprawdzKolizje(VK_LEFT);
			if(positionX>MIN_X +(trzymaKlocek?15:0) && (!kolizja || positionY < MAX_Y-25))
				PoruszajDzwig(hdc,-3,0,hWnd);
            break;
        }
		case VK_RIGHT:
        {
			bool kolizja=sprawdzKolizje(VK_RIGHT);
			if(positionX<MAX_X && (!kolizja || positionY < MAX_Y-25))
				PoruszajDzwig(hdc,3,0,hWnd);
            break;
        }
		case VK_DOWN:
        {
			bool kolizja=sprawdzKolizje(VK_DOWN);
			if(positionY<MAX_Y && !kolizja)
				PoruszajDzwig(hdc,0,3,hWnd);
            break;
        }
		case VK_UP:
        {
			if(positionY>MIN_Y)
				PoruszajDzwig(hdc,0,-3,hWnd);
			break;
        }
    }
	EndPaint(hWnd, &ps);
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
