// Quick Note.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Quick Note.h"
#include <commctrl.h>
#include <Windowsx.h>

#define MAX_LOADSTRING 100
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//My variables
HWND hWndTagList;
HWND hWndEdtNote;
HWND hWndNoteList;
HWND hWndBtnSave;
HWND hWndCBBTag;
MANAGER *Manager;

wstring edtTagStr;
bool updateTagStr = false;


NOTIFYICONDATA nidApp;
HMENU hPopMenu;
//My prototypes
int OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int OnBtnSave(MANAGER *Manager, HWND hWnd);
int OnBtnNewNote(HWND hWndTagList, HWND hWndNoteList, HWND hWndEdtNote, HWND hWndCBBTag);
int updateLBTag(MANAGER *Manager, HWND hWndLB, HWND hWndCBBTag);
int updateLBNotes(MANAGER *Manager, HWND hWndLB);
int updateEdtNote(HWND hWndEdtNote, HWND hWndCBBTag, NOTE note);
int getCurrSltedTagIndex(MANAGER *Manager, HWND hWndTagList);
int getCurrSelectedNoteID(MANAGER *Manager, HWND hWndTagList, HWND hWndNoteList);
int getCurrSltedTagIndexInList(MANAGER *Manager, HWND hWndTagList);
static RECT createRect(LONG left, LONG top, LONG right, LONG bottom);
float percentOf(TAG tag, vector<TAG> tagList);
int drawBar(HDC hdc, int x, int y, int width, int height, HBRUSH hbrush);
int drawChart(HWND hWnd, HFONT hFont, HDC hdc, vector<TAG> tagList, int xTop, int yTop, int fullBarWidth, int barHeight, int distance);
bool sortTagList(TAG tag1, TAG tag2);
void doInstallHook(HWND hWnd);
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Statistic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
///


///
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
    LoadStringW(hInstance, IDC_QUICKNOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKNOTE));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKNOTE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
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
      CW_USEDEFAULT, 0, 800, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   HICON hMainIcon;
   hMainIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_ICON2));

   nidApp.cbSize = sizeof(NOTIFYICONDATA); // sizeof the struct in bytes 
   nidApp.hWnd = (HWND)hWnd;              //handle of the window which will process this app. messages 
   nidApp.uID = IDI_ICON2;           //ID of the icon that will appear in the system tray 
   nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //ORing of all the flags 
   nidApp.hIcon = hMainIcon; // handle of the Icon to be displayed, obtained from LoadIcon 
   nidApp.uCallbackMessage = TRAY_ICON_NOTIFICATION;
   LoadString(hInstance, IDS_APP_TITLE, nidApp.szTip, MAX_LOADSTRING);
   Shell_NotifyIcon(NIM_ADD, &nidApp);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (updateTagStr)
	{
		SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)edtTagStr.c_str());
		updateTagStr = false;
	}
	switch (message)
	{
	case TRAY_ICON_NOTIFICATION:
		// systray msg callback 
		switch (LOWORD(lParam))
		{
		case WM_LBUTTONDBLCLK: // to open the GUI of the Application
			ShowWindow(hWnd, SW_NORMAL);
			return TRUE;
		case WM_RBUTTONDOWN:
			POINT  lpClickPoint;
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			GetCursorPos(&lpClickPoint);
			hPopMenu = CreatePopupMenu();
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDC_TRAY_ICON_SHOW, _T("View notes")); // notes
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDC_BTN_STATISTIC, _T("View statistics")); // statistics		
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, L"Exit");

			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
			return TRUE;

		}
		break;

	case WM_CREATE:
	{
		OnCreate(hWnd, message, wParam, lParam);
		break;
	}
	
		
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		

		switch (wmId)
		{
		case IDC_CBBOX_TAG:
			{
				if (HIWORD(wParam) == CBN_SELCHANGE)
				{
					int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
					TCHAR buff[1024];
					//wchar_t* buffer = new wchar_t[length + 1];
					SendMessage(hWndCBBTag, WM_GETTEXT, sizeof(buff), (LPARAM)buff);
					edtTagStr = buff;

					SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)buff);

					edtTagStr += L", ";
					edtTagStr += buff;
					edtTagStr = Manager->TagListToString(Manager->strToTagList(edtTagStr));

					SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)edtTagStr.c_str());
					updateTagStr = true;
				}

			}
			break;
		case IDC_EDIT_NOTE://your edit control ID
		{
			if (HIWORD(wParam) == EN_CHANGE) //notification)   
			{
				InvalidateRect(hWndEdtNote, NULL, TRUE);
			}
			break;
		}
		case IDC_BTN_NEW_NOTE:
		{
			OnBtnNewNote(hWndTagList, hWndNoteList, hWndEdtNote, hWndCBBTag);
			break;
		}
		case IDC_BTN_SAVE:
		{
			OnBtnSave(Manager, hWnd);
			break;
		}
		case IDC_LISTBOX_TAG:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				updateLBNotes(Manager, hWndNoteList);
				break;
			}
			}
			break;
		}
		case IDC_LISTBOX_NOTE_LIST:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				int currSelectedNoteID = getCurrSelectedNoteID(Manager, hWndTagList, hWndNoteList);
				if (currSelectedNoteID == LB_ERR)
				{
					//mess
					SendMessage(hWndEdtNote, WM_SETTEXT, 0, (LPARAM)L"");
					SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)L"");
					break;
				}
				else
				{
					updateEdtNote(hWndEdtNote, hWndCBBTag, Manager->NoteList[currSelectedNoteID]);
				}
				break;
			}
			}
			break;
		}
		case IDC_TRAY_ICON_SHOW:
		{
			ShowWindow(hWnd, SW_NORMAL);
			break;
		}
		case IDC_BTN_STATISTIC:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_STATISTIC), hWnd, Statistic);
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
	}
	break;
	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nidApp);
		PostQuitMessage(0);
		break;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, TRANSPARENT);

		HBRUSH backgroundColor = CreateSolidBrush(RGB(255, 255, 255));
		return (LRESULT)backgroundColor; // or any other brush you want
	}
	break;
	case WM_SYSCOMMAND:
		if (SC_CLOSE == wParam)
		{
			ShowWindow(hWnd, SW_HIDE); //hWnd is the handle of the application window
			return TRUE;
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CTLCOLOREDIT:
		if ((HWND)lParam == hWndEdtNote /*|| (HWND)lParam == hWndCBBTag*/) {
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);

			HBRUSH backgroundColor = CreateSolidBrush(RGB(255, 255, 141));
			return (LRESULT)backgroundColor; // or any other brush you want
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CTLCOLORLISTBOX:
	{
		if ((HWND)lParam == hWndNoteList || (HWND)lParam == hWndTagList) {
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);

			HBRUSH backgroundColor = CreateSolidBrush(RGB(255, 255, 141));
			return (LRESULT)backgroundColor; // or any other brush you want
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
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
//Statistic diaglog
INT_PTR CALLBACK Statistic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		HFONT hFont_Title = CreateFont(30, 0, 0, 0, 1550, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Courier New"));

		HWND hWndStatic = CreateWindow(L"Static", L"TOP 5 USED TAGS", WS_CHILD | WS_VISIBLE| SS_CENTER, 0, 30, 800, 30, hDlg, NULL, hInst, NULL);

		SendMessage(hWndStatic, WM_SETFONT, (WPARAM)hFont_Title, TRUE);

		HFONT hFont = CreateFont(16, 0, 0, 0, 550, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Courier New"));

		//sort
		vector<TAG> tagList = Manager->TagList;
		sort(tagList.begin(), tagList.end(), sortTagList);
		
		drawChart(hDlg, hFont, hdc, tagList, 50, 100, 350, 25, 25);
		break;
	}
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


int OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	doInstallHook(hWnd);
	RECT rect;
	if (GetWindowRect(hWnd, &rect))
	{
		Manager = new MANAGER();
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		//BTN
		HWND hWndBtnNewNote = CreateWindow(L"Button", L"New Note", WS_CHILD | WS_VISIBLE, 40, 20, 150, 30, hWnd, (HMENU) IDC_BTN_NEW_NOTE, hInst, NULL);
		HWND hWndBtnStatistic = CreateWindow(L"Button", L"Statistics ", WS_CHILD | WS_VISIBLE, 300, 20, 150, 30, hWnd, (HMENU)IDC_BTN_STATISTIC, hInst, NULL);

		HWND hWndStaticTagList = CreateWindow(L"Static", L"Tags list:", WS_CHILD | WS_VISIBLE, 10, 70, 150, 20, hWnd, NULL, hInst, NULL);
		HWND hWndStaticNoteList = CreateWindow(L"Static", L"Notes list:", WS_CHILD | WS_VISIBLE, 170, 70, 150, 20, hWnd, NULL, hInst, NULL);
		HWND hWndStaticCurrNote = CreateWindow(L"Static", L"Current Note:", WS_CHILD | WS_VISIBLE, 330, 70, 150, 20, hWnd, NULL, hInst, NULL);
		HWND hWndStaticAddTag = CreateWindow(L"Static", L"Tag:", WS_CHILD | WS_VISIBLE, 330, height - 100, width - 350, 20, hWnd, NULL, hInst, NULL);
		

		hWndTagList = CreateWindowEx(WS_EX_CLIENTEDGE, L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY |LBS_SORT, 10, 100, 100, height - 160, hWnd, (HMENU)IDC_LISTBOX_TAG, hInst, NULL);
		hWndNoteList = CreateWindowEx(WS_EX_CLIENTEDGE, L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 120, 100, 200, height - 160, hWnd, (HMENU)IDC_LISTBOX_NOTE_LIST, hInst, NULL);
		hWndEdtNote = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE, 330, 100, width - 350, height - 210, hWnd, (HMENU)IDC_EDIT_NOTE, hInst, NULL);
		//hWndCBBTag = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"", WS_CHILD | WS_VISIBLE, 330, height - 105, width - 450, 25, hWnd, (HMENU)IDC_EDIT_TAG, hInst, NULL);
		hWndCBBTag = CreateWindowEx(WS_EX_STATICEDGE, L"COMBOBOX", L"MyCombo1", CBS_DROPDOWN | WS_HSCROLL | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE, 330, height - 105, width - 450, 25, hWnd, (HMENU)IDC_CBBOX_TAG, hInst, NULL); // 100 = ID of this control
		
		
		hWndBtnSave = CreateWindowEx(0, L"Button", L"Save", WS_CHILD | WS_VISIBLE, 330 + width - 440, height - 105, 90, 25, hWnd, (HMENU)IDC_BTN_SAVE, hInst, NULL);
		SendMessage(hWndCBBTag, EM_SETCUEBANNER, FALSE, (LPARAM)L"Set tags");

		HFONT hFont = CreateFont(18, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"));

		SendMessage(hWndTagList, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndNoteList, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndEdtNote, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndCBBTag, WM_SETFONT, (WPARAM)hFont, TRUE);

		hFont = CreateFont(17, 0, 0, 0, 600, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));

		SendMessage(hWndStaticTagList, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndStaticNoteList, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndStaticCurrNote, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndBtnNewNote, WM_SETFONT, (WPARAM)hFont, TRUE);
		
		/*int pos = (int)SendMessage(hWndTagList, LB_ADDSTRING, 0, (LPARAM)L"mot con vit");
		SendMessage(hWndTagList, LB_ADDSTRING, 0, (LPARAM)L"1");
		SendMessage(hWndTagList, LB_ADDSTRING, 0, (LPARAM)L"2");
		SendMessage(hWndTagList, LB_ADDSTRING, 0, (LPARAM)L"3");
		SendMessage(hWndTagList, LB_ADDSTRING, 0, (LPARAM)L"4");*/
		int code = Manager->loadFromFile();
		/*wstringstream ss;
		ss << code;
		MessageBox(hWnd, ss.str().c_str(), L"", MB_OK);*/
		updateLBTag(Manager, hWndTagList, hWndCBBTag);

		
		
		return 0;
	}
}

//-----Buttons
int OnBtnSave(MANAGER *Manager, HWND hWnd)
{
	WCHAR noteData[2000];
	WCHAR tagData[400];
	GetWindowText(hWndEdtNote, noteData, sizeof(noteData));
	GetWindowText(hWndCBBTag, tagData, sizeof(tagData));

	int currSelectedNote = getCurrSelectedNoteID(Manager, hWndTagList, hWndNoteList);
	if (currSelectedNote != LB_ERR)//overwrite current selected note
	{
		Manager->saveNoteToList(Manager->strToTagList(tagData), noteData, currSelectedNote);
	}
	else//new note
	{
		Manager->saveNoteToList(Manager->strToTagList(tagData), noteData);
	}

	int err = Manager->saveToFile();
	if (err != 0)
	{
		wstringstream ss;
		ss << L"Can't open file!\n Err code = " << err;
		MessageBox(hWnd, ss.str().c_str(), L"Notification", MB_OK);
	}
	else
	{
		MessageBox(hWnd, L"Saved!", L"Notification", MB_OK);
		int currSelectedTag = getCurrSltedTagIndexInList(Manager, hWndTagList);
		int noteIndexInTag = (int)SendMessage(hWndNoteList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		updateLBTag(Manager, hWndTagList, hWndCBBTag);
		if (currSelectedTag != LB_ERR)
		{
			SendMessage(hWndTagList, LB_SETCURSEL, (WPARAM)currSelectedTag, (LPARAM)0);
			updateLBNotes(Manager, hWndNoteList);
			SendMessage(hWndNoteList, LB_SETCURSEL, (WPARAM)noteIndexInTag, (LPARAM)0);

			int currSelectedNoteID = getCurrSelectedNoteID(Manager, hWndTagList, hWndNoteList);
			if (currSelectedNoteID != LB_ERR)
			{
				updateEdtNote(hWndEdtNote, hWndCBBTag, Manager->NoteList[currSelectedNoteID]);
			}
		}
		else
		{
			SendMessage(hWndEdtNote, WM_SETTEXT, 0, (LPARAM)L"");
			SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)L"");

		}
	}
	return err;
}
int OnBtnNewNote(HWND hWndTagList, HWND hWndNoteList, HWND hWndEdtNote, HWND hWndCBBTag)
{
	SendMessage(hWndTagList, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	SendMessage(hWndNoteList, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
	SendMessage(hWndEdtNote, WM_SETTEXT, 0, (LPARAM)L"");
	SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)L"");
	return 0;
}

int updateLBTag(MANAGER *Manager, HWND hWndLB, HWND hWndCBBTag)
{
	SendMessage(hWndLB, LB_RESETCONTENT, NULL, NULL);
	wstringstream ss;
	for (int i = 0; i < Manager->TagList.size(); i++)
	{
		wstringstream().swap(ss);
		ss << Manager->TagList[i].TagName << "(" << Manager->TagList[i].Id.size() << ")";

		SendMessage(hWndLB, LB_ADDSTRING, 0, (LPARAM)ss.str().c_str());
	}

	//update dropdown list of tagbox
	TCHAR buff[1024];
	SendMessage(hWndCBBTag, WM_GETTEXT, sizeof(buff), (LPARAM)buff);
	edtTagStr = buff;
	SendMessage(hWndCBBTag, CB_RESETCONTENT, 0, 0);

	//sort tag
	vector<TAG> tagList = Manager->TagList;
	sort(tagList.begin(), tagList.end(), sortTagList);

	//add to dropdown list
	if (tagList.size() > 1)
	{
		for (int i = 1; i < tagList.size(); i++)
		{
			SendMessage(hWndCBBTag, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tagList[i].TagName.c_str());
		}
	}

	
	edtTagStr += L", ";
	edtTagStr += buff;
	edtTagStr = Manager->TagListToString(Manager->strToTagList(edtTagStr));

	SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)edtTagStr.c_str());

	return 0;
}
int updateLBNotes(MANAGER *Manager, HWND hWndLB)
{
	int currSelectedTag = getCurrSltedTagIndex(Manager, hWndTagList);
	wstring selectedTagStr = Manager->TagList[currSelectedTag].TagName;
	SendMessage(hWndLB, LB_RESETCONTENT, NULL, NULL);

	int tagPos = -1;
	for (int i = 0; i < Manager->TagList.size(); i++)
	{
		if (Manager->TagList[i].TagName == selectedTagStr)
		{
			tagPos = i;
			break;
		}
	}

	//Load content
	if (tagPos != -1)
	{
		wstring wsNotePreview;
		int noteId;
		for (int i = 0; i < Manager->TagList[tagPos].Id.size(); i++)
		{
			noteId = Manager->TagList[tagPos].Id[i];
			wsNotePreview = Manager->NoteList[noteId].Content.substr(0, 40);
			wsNotePreview += L"...";
			
			SendMessage(hWndLB, LB_ADDSTRING, 0, (LPARAM)wsNotePreview.c_str());
		}
	}
	//
	SendMessage(hWndEdtNote, WM_SETTEXT, 0, (LPARAM)L"");
	SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)L"");
	return 0;
}
int updateEdtNote(HWND hWndEdtNote, HWND hWndCBBTag, NOTE note)
{
	SendMessage(hWndEdtNote, WM_SETTEXT, 0, (LPARAM)note.Content.c_str());

	wstring tag;
	for (int i = 0; i < note.Tag.size(); i++)
	{
		tag += note.Tag[i] + L", ";
	}

	tag = Manager->TagListToString(Manager->strToTagList(tag));
	SendMessage(hWndCBBTag, WM_SETTEXT, 0, (LPARAM)tag.c_str());

	return 0;
}
int getCurrSltedTagIndex(MANAGER *Manager, HWND hWndTagList)
{
	// Get selected index.
	int itemIndex = (int)SendMessage(hWndTagList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (itemIndex == LB_ERR)// No selection
	{
		return LB_ERR;
	}
	WCHAR selectedTagStr[TAG_MAX_LENGTH];
	
	// Get item data.
	int i = (int)SendMessage(hWndTagList, LB_GETTEXT, itemIndex, (LPARAM)selectedTagStr);

	wstring wsSelectedTagStr = selectedTagStr;
	
	while (wsSelectedTagStr[wsSelectedTagStr.size()-1] != '(')
	{
		wsSelectedTagStr.pop_back();
	}
	wsSelectedTagStr.pop_back();

	//find match tag in tags list
	int tagPos = -1; 
	for (int i = 0; i < Manager->TagList.size(); i++)
	{
		if (Manager->TagList[i].TagName == wsSelectedTagStr)
		{
			tagPos = i;
			break;
		}
	}

	return tagPos;
}
int getCurrSelectedNoteID(MANAGER *Manager, HWND hWndTagList, HWND hWndNoteList)
{
	int tagIndex = getCurrSltedTagIndex(Manager, hWndTagList);

	
	int noteIndexInTag = (int)SendMessage(hWndNoteList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (noteIndexInTag == LB_ERR)
	{
		return LB_ERR;
	}

	return Manager->TagList[tagIndex].Id[noteIndexInTag];
}
int getCurrSltedTagIndexInList(MANAGER *Manager, HWND hWndTagList)
{
	// Get selected index.
	int itemIndex = (int)SendMessage(hWndTagList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (itemIndex == LB_ERR)// No selection
	{
		return LB_ERR;
	}
	return itemIndex;
}

//--------------draw chart funcs
int drawChart(HWND hWnd, HFONT hFont, HDC hdc, vector<TAG> tagList, int xTop, int yTop, int fullBarWidth, int barHeight, int distance)
{
	int txtBoxWidth = 300;
	int yText = yTop;
	int yBar = yTop;
	int xBar = xTop + txtBoxWidth + 10;
	int realBarWidth = 0;

	int Red = 230, Green = 200, Blue = 75;
	HBRUSH hbrush = CreateSolidBrush(RGB(Red, Green, Blue));

	//txtBoxWidth -= 70;// for percent text

	//begin draw chart
	wstringstream wss;
	wss.imbue(locale(""));

	int loopTime = tagList.size();
	if (loopTime > 6)//show top 5 most used tags
	{
		loopTime = 6;
	}
	for(int i = 1; i <loopTime; i++)
	{
		float per = percentOf(tagList[i], tagList);
		realBarWidth = fullBarWidth*per + 3;//i want draw a line even if percent = 0

		drawBar(hdc, xBar, yBar, realBarWidth, barHeight, hbrush);

		//write text
		wss.str(L"");//clear stream
		//if (tagList[i].TagName != NULL)
		//{
			wss << tagList[i].TagName << ":\0";
			//chart_HWND.htxtCategory[i] = 
			HWND hWndName = CreateWindowEx(0, L"Static", wss.str().c_str(), WS_VISIBLE | WS_CHILD, xTop, yBar, txtBoxWidth, barHeight, hWnd, NULL, NULL, NULL);

		//}

		wss.str(L"");
		wss << " " << tagList[i].Id.size() << "(" << fixed << setprecision(0) << per * 100 << "%)\0";
		//if (chart_HWND.htxtPercent[i] == NULL)
		//{
			//chart_HWND.htxtPercent[i] = 
			HWND hWndLast = CreateWindowEx(0, L"Static", wss.str().c_str(), WS_VISIBLE | WS_CHILD, xBar + realBarWidth, yBar, txtBoxWidth, barHeight, hWnd, NULL, NULL, NULL);
		//}
		//else
		//{
		//	DestroyWindow(chart_HWND.htxtPercent[i]);
		//	chart_HWND.htxtPercent[i] = CreateWindowEx(0, L"Static", wss.str().c_str(), WS_VISIBLE | WS_CHILD, xBar + realBarWidth, yBar, txtBoxWidth, barHeight, hWnd, NULL, NULL, NULL);
		//	//SetWindowText(chart_HWND.htxtPercent[i], wss.str().c_str());
		//}

		//setfont
		SendMessage(hWndName, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hWndLast, WM_SETFONT, (WPARAM)hFont, TRUE);

		//generate new brush color for next bar
		Red -= 0;
		Green -= 25;
		Blue -= 10;
		hbrush = CreateSolidBrush(RGB(Red, Green, Blue));

		yText += barHeight + distance;
		yBar += barHeight + distance;
	}

	return 0;
}
int drawBar(HDC hdc, int x, int y, int width, int height, HBRUSH hbrush)
{
	RECT rect = createRect(x, y, x + width, y + height);
	FillRect(hdc, &rect, hbrush);

	return 0;
}
float percentOf(TAG tag, vector<TAG> tagList)
{
	if (tag.Id.size() == 0)
	{
		return 0;
	}


	long double sum = 0;
	for (int i = 1; i < tagList.size(); i++)
	{
		sum += tagList[i].Id.size();
	}

	float result = tag.Id.size() *1.0/ sum;
	return  result;
}
static RECT createRect(LONG left, LONG top, LONG right, LONG bottom)
{
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	return rect;
}
bool sortTagList(TAG tag1, TAG tag2) { return tag1.Id.size() > tag2.Id.size(); }
//--------------draw chart funcs

void doInstallHook(HWND hWnd)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"HookFunction.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_installKeyBoardHook");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}