// Lab1-1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab1-1.h"
#include "BrokenLine.h"
#include "EllipseFigure.h"
#include "PrintRect.h"
#include "TextRect.h"
#include <list>
#include <iterator>
#include <commdlg.h>
#include <CommCtrl.h>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

list<AbstractFigure*> lFigure;
AbstractFigure *tmpFigure = NULL;
RECT* rCanvas = new RECT();
int shapeType = 0;
bool mouseDown = false;
HENHMETAFILE hEnhMtf;
COLORREF color1 = RGB(0, 0, 0);
COLORREF color2 = RGB(255, 255, 255);
int iBrushSize = 1;
double dScale = 1;
POINT pMouseDown;
PRINTDLG pd;
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

///////// для зума

/**/


string GetFilepathDialog(HWND hWnd, bool save) {
	static OPENFILENAME ofn;
	static char fullpath[255], filename[256], dir[256];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst; // дескриптор копии приложения
	ofn.lpstrFilter = (LPCSTR)"Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPSTR)fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = (LPSTR)filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = (LPCSTR)dir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
	if (save) {
		ofn.lpstrTitle = (LPCSTR)"Save picture as...";
		GetSaveFileName(&ofn);
	}
	else {
		ofn.lpstrTitle = (LPCSTR)"Open picture";
		GetOpenFileName(&ofn);
	}
	return fullpath;
}

void OpenPicture(HWND hWnd, HDC hdc, RECT rect) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 0);
	if (!fullpath.empty())
	{
		//GetClientRect(hWnd, &rect);
		hEnhMtf = GetEnhMetaFile((LPCSTR)fullpath.c_str());
		PlayEnhMetaFile(hdc, hEnhMtf, &rect);
	}
}

void SavePicture(HWND hWnd, HDC hdc, RECT rect) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 1);
	if (!fullpath.empty())
	{
		//	HENHMETAFILE hEnhMtf;
		HDC mtfHdc;
		fullpath += ".emf";
		///
		/*RECT rect1;
		rect1.top = 0;
		rect1.left = 0;
		rect1.bottom = 100;
		rect1.right=100;*/
		///
		mtfHdc = CreateEnhMetaFile(hdc, (LPCSTR)fullpath.c_str(), &rect, NULL);
		/*(&rect)->top /= 26;
		(&rect)->left /= 26;
		(&rect)->left -= rCanvas->left;
		(&rect)->bottom /= 26;
		(&rect)->right /= 26;
		(&rect)->right -= rCanvas->left;*/
		PlayEnhMetaFile(mtfHdc, hEnhMtf, &rect);
		for (AbstractFigure *figure : lFigure)
			figure->Draw(mtfHdc, dScale);
		hEnhMtf = CloseEnhMetaFile(mtfHdc);
		DeleteEnhMetaFile(hEnhMtf);
	}
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
   HWND bLine = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 30, 30, 30, hWnd, (HMENU)LINE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   HBITMAP bmpSource = NULL;
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\line.bmp", 
	   IMAGE_BITMAP, 0,0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bLine, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);// set button image
   //Pencil
   HWND bPencil = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 30, 30, 30, hWnd, (HMENU)PENCIL_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\pencil.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bPencil, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Broken
   HWND bBroken = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 70, 30, 30, hWnd, (HMENU)BROKEN_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\broken.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bBroken, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Rectangle
   HWND bRectangle = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 70, 30, 30, hWnd, (HMENU)RECTANGLE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\rectangle.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bRectangle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Ellipse
   HWND bEllipse = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 110, 30, 30, hWnd, (HMENU)ELLIPSE_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\ellipse.bmp", 
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bEllipse, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //ChooseColor1
   HWND bChooseColor1 = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 110, 30, 30, hWnd, (HMENU)CHOOSECOLOR_BUTTON1,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\color.bmp",
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bChooseColor1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //ChooseColor2
   HWND bChooseColor2 = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 70, 150, 30, 30, hWnd, (HMENU)CHOOSECOLOR_BUTTON2,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\color.bmp",
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bChooseColor2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);
   //Text
   HWND bText = CreateWindow("BUTTON",
	   "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP, 30, 150, 30, 30, hWnd, (HMENU)TEXT_BUTTON,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);
   bmpSource = (HBITMAP)LoadImage(NULL, "Y:\\Учеба-5\\ОСИСП\\OSISP-1\\Lab1-1\\Img\\text.bmp",
	   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bmp image
   SendMessage(bText, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpSource);


   //Pen size combobox
   HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
	   CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
	   30, 190, 70, 230, hWnd, NULL, hInstance,
	   NULL);
   TCHAR Sizes[4][3] =
   {
	   TEXT("1"), TEXT("3"), TEXT("7"), TEXT("10")
   };

   TCHAR A[3];
   memset(&A, 0, sizeof(A));
   for (int k = 0; k <= 3; k += 1)
   {
	  strcpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Sizes[k]);
	   SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
   }
   SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  абрабодчиег
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
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			//MessageBox(hWnd, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);
			iBrushSize = atoi(ListItem);
		}
			
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case LINE_BUTTON: case IDM_LINE:
			{
				shapeType = 1;
			}
			break;
			case RECTANGLE_BUTTON: case IDM_RECTANGLE:
			{
				shapeType = 3;
			}
			break;
			case PENCIL_BUTTON: case IDM_PEN:
			{
				shapeType = 2;
			}
			break;
			case ELLIPSE_BUTTON: case IDM_ELLIPSE:
			{
				shapeType = 4;
			}
			break;
			case BROKEN_BUTTON: case IDM_POLYLINE:
			{
				shapeType = 5;
			}
			break;
			case IDM_PAN:
			{
				shapeType = 7;
			}
			break;
			case IDM_PRINT:
			{
				shapeType = 8;
			}
			break;
			case TEXT_BUTTON: case IDM_TEXT:
			{
				shapeType = 6;
				SetFocus(hWnd);
			}
			break;
			case CHOOSECOLOR_BUTTON1: case IDM_COLOR1:
			{
				color1 = choseColor(hWnd);
			}
			break;
			case CHOOSECOLOR_BUTTON2: case IDM_COLOR2:
			{
				color2 = choseColor(hWnd);
			}
			break;
			case IDM_SAVE:
			{
				HDC hdc = GetDC(hWnd);
				SavePicture(hWnd, hdc, *rCanvas);
			}
			break;
			case IDM_OPEN:
			{
				HDC hdc = GetDC(hWnd);
				OpenPicture(hWnd, hdc, *rCanvas);
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
			case 6:
			{
				tmpFigure = new TextRect();
				tmpFigure->x1 = p.x;
				tmpFigure->y1 = p.y;
			}
			break;
			case 8:
			{
				tmpFigure = new PrintRect();
				tmpFigure->x1 = p.x;
				tmpFigure->y1 = p.y;
			}
			break;
			case 7:
				if (wParam & MK_SHIFT)
					pMouseDown = p;
				break;
			}
			if (tmpFigure != NULL) {
				tmpFigure->qColor = color1;
				tmpFigure->qColor2 = color2;
				tmpFigure->iBrushSize = iBrushSize;
			}
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
			case 1: case 3: case 4: case 8:
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
			if ((shapeType != 5)&&(shapeType != 6) && (shapeType != 8)) {
				lFigure.push_back(tmpFigure);
				tmpFigure = NULL;
			}
			if (shapeType == 8)
			{
				HDC hdc = GetDC(hWnd);
				RECT* rect = new RECT();
				rect->top = tmpFigure->y1*26;
				rect->left = (tmpFigure->x1 - rCanvas->left) *26;
				rect->bottom = tmpFigure->y2 *26 ;
				rect->right = (tmpFigure->x2 - rCanvas->left) *26;
				SavePicture(hWnd, hdc, *rect);
				tmpFigure = NULL;
			}
			InvalidateRect(hWnd, rCanvas, NULL);
		}
		else
		{
			if (shapeType == 7)
				mouseDown = false;
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);
		if(mouseDown)
		if ((tmpFigure != NULL))
		{
			
			
			switch (shapeType)
			{
			case 1: case 3: case 4: case 8:
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
			}	
			InvalidateRect(hWnd, rCanvas, NULL);
		}
		else
			if (shapeType == 7)
			{
				POINT pDelta;
				if (wParam & MK_SHIFT) {
					pDelta.x = pMouseDown.x - p.x;
					pDelta.y = pMouseDown.y - p.y;
					pMouseDown = p;
					for (AbstractFigure *figure : lFigure)
						figure->Pan(pDelta);
					InvalidateRect(hWnd, rCanvas, NULL);
				}
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
	//KEY PRESS
	case WM_CHAR:
	{
		if (shapeType == 6)
		{
			switch (wParam)
			{
			case 0x08:
			{
				((TextRect*)tmpFigure)->RemoveChar();
			}
			break;
			case 0x0D:
			{
				lFigure.push_back(tmpFigure);
				tmpFigure = NULL;
			}
			break;
			default:
				((TextRect*)tmpFigure)->AddChar((TCHAR)wParam);
				break;
			}
			InvalidateRect(hWnd, rCanvas, NULL);
		}
		
	}
	break;
	case WM_MOUSEWHEEL:
	{
		if (wParam & MK_CONTROL)
		{
			if ((GET_WHEEL_DELTA_WPARAM(wParam) > 0) && (dScale < 2)) {
				dScale += 0.1;
			}
			if ((GET_WHEEL_DELTA_WPARAM(wParam) < 0) && (dScale > 1)) {
				dScale -= 0.1;
			}
			InvalidateRect(hWnd, rCanvas, TRUE);
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
			/*XFORM xForm;
			xForm.eM11 = xForm.eM22 = dScale;
			xForm.eM12 = xForm.eM21 = xForm.eDx = xForm.eDy = 0;
			SetGraphicsMode(hdc, GM_ADVANCED);
			SetWorldTransform(hdc, &xForm);*/
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
				figure->Draw(hdc2, dScale);
			if (tmpFigure != NULL)
				tmpFigure->Draw(hdc2, dScale);
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
		lFigure.clear();
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
