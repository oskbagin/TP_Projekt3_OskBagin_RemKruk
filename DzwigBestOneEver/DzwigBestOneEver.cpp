// DzwigBestOneEver.cpp : Defines the entry point for the application.
//

// headers
#include "stdafx.h"
#include "DzwigBestOneEver.h"
#include "Klocuszek.h"

// consts
#define MAX_LOADSTRING 100
const int MaxKlockow=10;
const int MAX_X=660;
const int MIN_X=255;
const int MAX_Y=467;
const int MIN_Y=155;
const int buttonPozX=1025;
const int buttonPozY=60;
const int buttonHeight=50;
const int buttonWidth=80;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
RECT drawArea1 = { 0, 0, 1024, 600 };
HWND hwndButton;
int positionX=450;
int positionY=155;
int ktory;
int ileKlockow;
bool trzymaKlocek;

Klocuszek Elementy[MaxKlockow];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void generujPoczatkoweUstawienie(Klocuszek*);
void rysujKlocuszki(HWND, Klocuszek*);
void dodajTrojkat(HWND);
void dodajKoleczko(HWND);
void dodajKwadrat(HWND);
void PoruszajDzwig(HDC, int, int, HWND);
int pozXwspolne(int, int=0);
void zlapKlocek(HWND, int);
void upuscKlocek(HWND);
void postawNaWiezy(int, HWND);
bool sprawdzKolizje(int);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	srand(time(0));
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
	LoadString(hInstance, IDC_DZWIGBESTONEEVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DZWIGBESTONEEVER));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DZWIGBESTONEEVER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DZWIGBESTONEEVER);
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
   generujPoczatkoweUstawienie(Elementy);							// poczatkowe ustawienie klocuszkow
   hInst = hInstance;												// Store instance handle in our global variable
   trzymaKlocek=false;

   /* przyciski do dodawania klckow na polu gry */
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1200, 768, NULL, NULL, hInstance, NULL);
   hwndButton = CreateWindow(TEXT("button"),                        // The class name required is button
		TEXT("Kwadrat"),											// the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,					    // the styles
		buttonPozX, buttonPozY,                                     // the left and top co-ordinates
		buttonWidth, buttonHeight,                                  // width and height
		hWnd,													    // parent window handle
		(HMENU)ID_BUTTON1,					 					    // the ID of your button
		hInstance,													// the instance of your application
		NULL);          

   hwndButton = CreateWindow(TEXT("button"), TEXT("Trójk¹t"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, buttonPozX, buttonPozY+60,
		buttonWidth, buttonHeight, hWnd, (HMENU)ID_BUTTON2, hInstance, NULL);          

   hwndButton = CreateWindow(TEXT("button"), TEXT("Kó³eczko"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, buttonPozX, buttonPozY+120,
		buttonWidth, buttonHeight, hWnd, (HMENU)ID_BUTTON3, hInstance, NULL);          

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
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON1:{
			if(ileKlockow<MaxKlockow)
				dodajKwadrat(hWnd);
			else ::MessageBox(hWnd, TEXT("Max 10 klocków!"), TEXT("BEEP"), NULL); 
			break;
		}
		case ID_BUTTON2:{
			if(ileKlockow<MaxKlockow)
				dodajTrojkat(hWnd);
			else ::MessageBox(hWnd, TEXT("Max 10 klocków!"), TEXT("BEEP"), NULL); 
			break;
		}
		case ID_BUTTON3:{
			if(ileKlockow<MaxKlockow)
				dodajKoleczko(hWnd);
			else ::MessageBox(hWnd, TEXT("Max 10 klocków!"), TEXT("BEEP"), NULL); 
			break;
		}
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
		InvalidateRect(hWnd, &drawArea1, FALSE);
		break;
				  }
	case WM_PAINT:
		// TODO: Add any drawing code here...
		hdc = BeginPaint(hWnd, &ps);
		PoruszajDzwig(hdc, 0, 0,hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		InvalidateRect(hWnd, &drawArea1, FALSE);
		hdc = BeginPaint(hWnd, &ps);

		switch (wParam) {
			case VK_SPACE:{
				int index=pozXwspolne(positionX);

				if(trzymaKlocek){
					if(index==-1)   upuscKlocek(hWnd);
					else			postawNaWiezy(index, hWnd);
				}
				else{
					zlapKlocek(hWnd, index);
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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
		return 0;
	}
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

void generujPoczatkoweUstawienie(Klocuszek* tab)
{
	ileKlockow=2;
	Ksztalty shapes[3]={ Kolko, Kwadrat, Trojkat };
	for(int i=0; i<ileKlockow; i++)
	{
		Elementy[i]=Klocuszek(shapes[rand()%3], MIN_X+rand()%(MAX_X-MIN_X), MAX_Y-15);
	}
}

void dodajTrojkat(HWND hWnd)
{
	ileKlockow++;
	Elementy[ileKlockow-1]=Klocuszek(Trojkat, MIN_X+rand()%(MAX_X-MIN_X), MAX_Y-15);
	::MessageBox(hWnd, TEXT("Dodano trójkat"), TEXT("Dodano"), NULL);
}

void dodajKwadrat(HWND hWnd)
{
	ileKlockow++;
	Elementy[ileKlockow-1]=Klocuszek(Kwadrat, MIN_X+rand()%(MAX_X-MIN_X), MAX_Y-15);
	InvalidateRect(hWnd, &drawArea1, FALSE);
	::MessageBox(hWnd, TEXT("Dodano kwadrat"), TEXT("Dodano"), NULL);
}

void dodajKoleczko(HWND hWnd)
{
	ileKlockow++;
	Elementy[ileKlockow-1]=Klocuszek(Kolko, MIN_X+rand()%(MAX_X-MIN_X), MAX_Y-15);
	InvalidateRect(hWnd, &drawArea1, FALSE);
	::MessageBox(hWnd, TEXT("Dodano kó³ko"), TEXT("Dodano"), NULL);
}

void rysujKlocuszki(HDC hdc, Klocuszek tab[])
{
	Graphics graphics(hdc);
	
	Pen      pen(Color(255, 0, 0, 255),2);

	for(int i=0; i<ileKlockow; i++){
		// Rysowanie trójk¹ta
		if(tab[i].getKsztalt() == TROJKAT && !tab[i].get_czyZlapany()){ 
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
   	  Image image(L"../grafika/dzwig_2.jpg");

	  graphics.DrawImage(&image, 10, 10);
	  rysujKlocuszki(hdcMem, Elementy);

	  if(!trzymaKlocek)
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

int pozXwspolne(int pozycja, int start)
{
	int i=start;
	for(i; i<ileKlockow; i++){
		if(i==ktory) continue;
		if(abs(Elementy[i].getX()+15-pozycja) < 15) return i;
	}
	return -1;
}

void zlapKlocek(HWND hWnd, int index)
{
	if(MAX_Y-positionY < 10){
		if(pozXwspolne(positionX)==-1 || !Elementy[index].get_moznaRuszyc());
		else if(Elementy[index].getKsztalt() != TROJKAT){
			::MessageBox(hWnd, TEXT("Mo¿na podnosiæ tylko trójk¹ty!"), NULL, NULL);
		}
		else{
			ktory=index;
			trzymaKlocek=true;
			Elementy[index].set_czyZlapany(true);
		}
	}
}

void upuscKlocek(HWND hWnd)
{
	if(positionY>MAX_Y-110){
		trzymaKlocek=false;
		Elementy[ktory].set_czyZlapany(false);
		Elementy[ktory].setY(MAX_Y+28);
		Elementy[ktory].setX(positionX-15);
		ktory=-1;
	}
	else ::MessageBox(hWnd, TEXT("Za wysoko by upuœciæ klocek!"), TEXT("UWAGA BHP!"), NULL);
}

void postawNaWiezy(int indexPod, HWND hWnd)
{
	if(Elementy[indexPod].getKsztalt()==TROJKAT){
		int k=Elementy[indexPod].getKNW();
		int temp;
		for( int i=indexPod; i<ileKlockow; i++){
			if(pozXwspolne(Elementy[i].getX(), i)){
				temp=Elementy[i].getKNW();
				if(temp>k) k=temp; }
		}

		if(positionY < MAX_Y-80) ::MessageBox(hWnd, TEXT("Ustawiono ju¿ trzy trójk¹ciki na wie¿y."), TEXT("!"), NULL);
		else{
			trzymaKlocek=false;
			Elementy[ktory].set_czyZlapany(false);
			Elementy[ktory].setX(positionX-15);
			Elementy[ktory].setY(Elementy[indexPod].getY()-30);
			Elementy[indexPod].set_moznaRuszyc(false);
		}
	}
}

bool sprawdzKolizje(int klawisz)
{
	if(!trzymaKlocek) return false;
	else{
		for(int i=0; i<ileKlockow; i++){
			if(i==ktory) continue;
			if(klawisz==VK_LEFT && abs(Elementy[i].getX()-positionX+42) < 5				// kolizja z prawej strony
				|| klawisz==VK_RIGHT && abs(Elementy[i].getX()-positionX-15) < 5		// kolizja z lewej strony
				|| klawisz==VK_DOWN && abs(Elementy[i].getX()-positionX+15)<25 && Elementy[i].getY()-positionY < 57) // kolizja z gory
			{ return true; }
		}
	}
	return false;
}