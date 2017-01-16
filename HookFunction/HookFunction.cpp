// HookFunction.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#define EXPORT _declspec(dllexport)

HHOOK hKBHook = NULL;
HINSTANCE hIntsKB;
KBDLLHOOKSTRUCT *kbStruct = NULL;
HWND hWndQuickNote = NULL;
extern "C" {
	LRESULT CALLBACK KeyBoardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode < 0)
			return CallNextHookEx(hKBHook, nCode, wParam, lParam);
		
		kbStruct = (KBDLLHOOKSTRUCT*)lParam;
		if ((GetAsyncKeyState(VK_LWIN) & 0x8000) != 0 || (GetAsyncKeyState(VK_RWIN) & 0x8000) != 0)
		{
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
			{
				ShowWindow(hWndQuickNote, SW_NORMAL);
			}
		}


		return CallNextHookEx(hKBHook, nCode, wParam, lParam);
	}
	EXPORT void _installKeyBoardHook(HWND hWnd)
	{
		if (hKBHook != NULL)
		{
			return;
		}
		hWndQuickNote = hWnd;
		hKBHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardHookProc, hIntsKB, 0);
		if (hKBHook)
		{
			//MessageBox(hWnd, L"Setup keyboard hook successfully!", L"Result", MB_OK);
		}
		else
		{
			//MessageBox(hWnd, L"Setup keyboard hook fail!", L"Result", MB_OK);
		}
		//return 0;
	}
}