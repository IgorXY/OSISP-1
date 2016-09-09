// Lab1-1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab1-1.h"
#include "BrokenLine.h"
#include "EllipseFigure.h"
#include "RectangleFigure.h"
#include <list>
#include <iterator>
#include <commdlg.h>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

list<RECT*> myList;
list<Line*> lLine;
list<AbstractFigure*> lFigure;
AbstractFigure *tmpFigure = NULL;
RECT* rCanvas = new RECT();
int shapeType = 0;
bool mouseDown = false;
COLORREF color = RGB(0, 0, 0);

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DSHADOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

COLORREF choseColor(HWND hWnd)
{
	CHOOSECOLOR color;
	COLORREF ccref[16];
	COLORREF selcolor = 0x000000;

	memset(&color, 0, sizeof(color));
	color.lStructSize = sizeof(CHOOSECOLOR);
	color.hwndOwner = hWnd;
	color.lpCustColors = ccref;
	color.rgbResult = selcolor;
	color.Flags = CC_RGBINIT;

	bool choosed = ChooseColor(&color);
	if (choosed)
	{
		selcolor = color.rgbResult;
	}
	return selcolor;
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   

   //InitializeButton(hWnd);
   //-----------CANVAS-----//
   rCanvas->left = CANVAS_X;
   rCanvas->top = CANVAS_Y;
   rCanvas->bottom = CANVAS_HEIGHT;
   rCanvas->right = CANVAS_WIDTH;
   //-------------MY BUTTONS--------------//
   //Line
   HWND bLine = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 30, 30, 30, hWnd, (HMENU)LINE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   HBITMAP bmpSource = NULL;
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\line.bmp", 
	   IMAGE_BITMAP, 0,0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bLine, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);// set button image
   //Pencil
   HWND bPencil = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 30, 30, 30, hWnd, (HMENU)PENCIL_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\pencil.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bPencil, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Broken
   HWND bBroken = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 70, 30, 30, hWnd, (HMENU)BROKEN_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\broken.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bBroken, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Rectangle
   HWND bRectangle = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 70, 30, 30, hWnd, (HMENU)RECTANGLE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\rectangle.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bRectangle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Ellipse
   HWND bEllipse = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 110, 30, 30, hWnd, (HMENU)ELLIPSE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\ellipse.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bEllipse, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //ChooseColor
   HWND bChooseColor = CreateWindow(L"BUTTON",
	   L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 110, 30, 30, hWnd, (HMENU)CHOOSECOLOR_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, L"Y:\\Ó÷åáà-5\\ÎÑÈÑÏ\\Lab1-1\\Img\\ellipse.bmp",
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bChooseColor, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  àáðàáîä÷èåã
//
//
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case LINE_BUTTON:
			{
				shapeType = 1; 
			}
			break;
			case RECTANGLE_BUTTON:
			{
				shapeType = 3;
			}
			break;
			case PENCIL_BUTTON:
			{
				shapeType = 2;
			}
			break;
			case ELLIPSE_BUTTON:
			{
				shapeType = 4;
			}
			break;
			case BROKEN_BUTTON:
			{
				shapeType = 5;
			}
			break;
			case CHOOSECOLOR_BUTTON:
			{			
				color = choseColor(hWnd);
			}			
			break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	//LEFT MOUSE BUTTON DOWN
	case WM_LBUTTONDOWN:
	{
		if (shapeType != 0)
		{
			
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			mouseDown = true;
		  /*  RECT *rect=new RECT();
			rect->left = p.x;
			rect->top = p.y;
			rect->bottom = p.y + 10;
			rect->right = p.x + 10;
			myList.push_back(rect);*/
			switch (shapeType)
			{
			case 1: 
			{
				tmpFigure = new Line();
				tmpFigure->x1 = p.x;
				tmpFigure->y1 = p.y;
			}
				break;
			case 2: 
				{
					tmpFigure = new Pencil();
					((Pencil*)tmpFigure)->AddPoint(p);
				}
				break;
			case 5:
			{
				if (tmpFigure == NULL)
					tmpFigure = new BrokenLine();
				((BrokenLine*)tmpFigure)->AddPoint(p);
			}
			break;
			case 3: 
			{
				tmpFigure = new RectangleFigure();
				tmpFigure->x1 = p.x;
				tmpFigure->y1 = p.y;
			}
			break;
			case 4:
			{
				tmpFigure = new EllipseFigure();
				tmpFigure->x1 = p.x;
				tmpFigure->y1 = p.y;
			}
				break;
			}
			tmpFigure->qColor = color;
			
			//SendMessage(hWnd, WM_PAINT, NULL, NULL);
		}
	}
	break;
	//LEFT MOUSE BUTTON UP
	case WM_LBUTTONUP:
	{
		if (tmpFigure != NULL)
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			switch (shapeType)
			{
			case 1: case 3: case 4:
			{
				tmpFigure->x2 = p.x;
				tmpFigure->y2 = p.y;
			}
			break;
		    case 2:
			{
				((Pencil*)tmpFigure)->AddPoint(p);
			}
			break;
			case 5:
			{
			//	((BrokenLine*)tmpFigure)->AddPoint(p);
			}
			break;
			}
			mouseDown = false;
			//lLine.push_back(line);
			if (shapeType != 5) {
				lFigure.push_back(tmpFigure);
				tmpFigure = NULL;
			}
			InvalidateRect(hWnd, rCanvas, NULL);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if(mouseDown)
		if (tmpFigure != NULL)
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			switch (shapeType)
			{
			case 1: case 3: case 4:
			{
				tmpFigure->x2 = p.x;
				tmpFigure->y2 = p.y;
			}
			break;
			case 2:
			{
				((Pencil*)tmpFigure)->AddPoint(p);
			}
			break;
			case 5:
			{
				//((BrokenLine*)tmpFigure)->AddPoint(p);
			}
			break;
			
			}

			
			InvalidateRect(hWnd, rCanvas, NULL);
		}
	}
	break;
	//LEFT MOUSE BUTTON DOWN
	case WM_RBUTTONDOWN:
	{
		if ((shapeType == 5) && (tmpFigure != NULL))
		{
			lFigure.push_back(tmpFigure);
			tmpFigure = NULL;
		}

	}
	break;

	//PAINT WHEN INVALIDATE
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			//RECT rect;
                   HDC hdc = BeginPaint(hWnd, &ps);
			//GetClientRect(hWnd, &rect);
			
			       //FillRect(hdc, rCanvas, CreateSolidBrush(RGB(255, 255, 255)));//clean canvas
			//SelectObject(hdc, GetStockObject(BLACK_PEN));
			       
			//HDC hdc = GetDC(hWnd);
			HDC hdc2 = CreateCompatibleDC(hdc);
			HBITMAP hbm = CreateCompatibleBitmap(hdc, CANVAS_WIDTH, CANVAS_HEIGHT);
			HANDLE hold = SelectObject(hdc2, hbm);

			COLORREF Scolor = RGB(255, 255, 255);
			HBRUSH brush = CreateSolidBrush(Scolor);
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));

			SelectObject(hdc2, brush);
			SelectObject(hdc2, pen);

			Rectangle(hdc2, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			DeleteObject(brush);
			DeleteObject(pen);
			//HBRUSH hBrush = CreateSolidBrush(RGB(50, 100, 255));
			//RedrawWidnow(hwnd, NULL, NULL, RDW_NOFRAME | RDW_VALIDATE);
			//FillRect(hdc, &rect,0);
			//HPEN hPen = CreatePen(PS_DASHDOTDOT, 2, NULL); SelectObject(hdc, hPen);
			//for (RECT *rect : myList)
				//Rectangle(hdc, rect->left, rect->top, rect->right, rect->bottom);
				//FillRect(hdc, rect, hBrush);
			for (AbstractFigure *figure : lFigure)
				figure->Draw(hdc2);
			if (tmpFigure != NULL)
				tmpFigure->Draw(hdc2);
            // TODO: Add any drawing code that uses hdc here...
			//DeleteObject(hPen);
                
			BitBlt(hdc, CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, hdc2, CANVAS_X, CANVAS_Y, SRCCOPY);
			SelectObject(hdc2, hold);
			DeleteObject(hbm);
			DeleteDC(hdc2);
			//ReleaseDC(hWnd, hdc);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		myList.clear();
        PostQuitMessage(0);
        break;
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
