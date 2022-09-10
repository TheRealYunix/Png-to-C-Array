#include <windows.h>
#include <cassert>
#include <iostream>
#include <string>
#include "types.h"
#include "CImage.hpp"


#include "resource.h"

HWND hList; //ListBox (Presents)

HWND TX2;

//Load
HWND TextLoad;
char ImageFilePath[2024];
char pngfile[256];
char cfile[256];

//Save
HWND TextSave;
char cfilePath[2024];

//Options
HWND IsHexadecimal;
bool IHchecked = false;
LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:

			if (LOWORD(wParam) == LoadBox){
					int x, y, c;
					std::string combine = "";


					ImageFilePath[0] = 0;
					OPENFILENAME LoadDL = {};
					LoadDL.lStructSize = sizeof(OPENFILENAME);
					LoadDL.lpstrFilter = "PNG-images (.png)\0*.png;\0JPG-images (.jpg)\0*.jpg;*.jpeg\0All Files\0*.*";
					LoadDL.lpstrCustomFilter = 0;
					LoadDL.nMaxCustFilter = 0;
					LoadDL.nFilterIndex = 0;
					LoadDL.lpstrFile = ImageFilePath;
					LoadDL.nMaxFile = 256;
					LoadDL.lpstrFileTitle = pngfile;
					LoadDL.nMaxFileTitle = 1024;
					LoadDL.lpstrTitle = "Select images to Load";
					LoadDL.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					

					if (GetOpenFileName(&LoadDL) != 0) {
						SetWindowText(TextLoad, pngfile);


						if(!stbi_load(ImageFilePath, &x, &y, &c, NULL)){
							combine = "ERROR";
						}
						combine = "\n\n";	combine += std::to_string(x); combine += "\n"; combine += std::to_string(y); combine +="\n"; combine += std::to_string(c);
						SetWindowText(TX2, combine.c_str());
					}
			}

			if (LOWORD(wParam) == SaveBox) {
				cfilePath[0] = 0;
				OPENFILENAME LoadDL = {};
				LoadDL.lStructSize = sizeof(OPENFILENAME);
				LoadDL.lpstrFilter = "H-Array (.h)\0";
				LoadDL.lpstrFile = cfilePath;
				LoadDL.nMaxFile = 256;
				LoadDL.lpstrFileTitle = cfile;
				LoadDL.nMaxFileTitle = 1024;
				LoadDL.lpstrInitialDir = cfilePath;
				LoadDL.lpstrTitle = "Select images to Load";
				LoadDL.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				if (GetSaveFileName(&LoadDL) != 0) SetWindowText(TextSave, cfile);
			}
			
			if (LOWORD(wParam) == IsHexadecimalCheckBox) 
			IHchecked = !IHchecked;


		//Check the Saves
		if (LOWORD(wParam) == ButtonCreate)
		{
			IMG image(ImageFilePath);
			image.IsHexadecimal = IHchecked;
			if (SendMessage(hList, LB_GETSEL, 0, 0) > 0) {
				image.present = 0;
				image.WriteCImage(cfilePath, pngfile);
			}
			if (SendMessage(hList, LB_GETSEL, 1, 0) > 0) {
				image.present = 1;
				image.WriteCImage(cfilePath, pngfile);
			}
			if (SendMessage(hList, LB_GETSEL, 2, 0) > 0) {
				image.present = 2;
				image.WriteCImage(cfilePath, pngfile);
			}
		}


		if (LOWORD(wParam) == CreditsBox) {
			MessageBoxA(hWnd, "Png to C Array Tool\nby Yunix! v0.2", "Credits", MB_ICONINFORMATION);
		}
		if (LOWORD(wParam) == ExitBox) {
			exit(0);
		}

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{






	HWND hWnd;

	HWND hButton;
	HWND cButton;

	WNDCLASS winw;
	MSG msg;

	HMENU hMenu = CreateMenu();

	//File Dialog
	HMENU hFile = CreateMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
	AppendMenu(hFile, MF_STRING, LoadBox, "Load");
	AppendMenu(hFile, MF_STRING, SaveBox, "Save");

	HMENU hAbout = CreateMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hAbout, "About");
	AppendMenu(hAbout, MF_STRING, CreditsBox, "Credits");
	AppendMenu(hAbout, MF_STRING, ExitBox, "Exit");

	winw = {};
	winw.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winw.style = CS_HREDRAW | CS_HREDRAW;
	winw.lpfnWndProc = MessageHandler;
	winw.hInstance = hInstance;
	winw.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	winw.lpszClassName = "PNGTOCARRAY";
	assert(RegisterClass(&winw));


	//Window
	hWnd = CreateWindow(winw.lpszClassName, "Png to C Array", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 280, 0, 0, hInstance, 0);

	/*Text fields*/
	HWND TX1 = CreateWindow("STATIC", "File info:\n\n Image Width:\n Image Height:\n Color Channels:\n", WS_VISIBLE | WS_CHILD | NULL, 20, 120, 260, 150, hWnd, NULL, hInstance, NULL);
	TX2 = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | NULL, 150, 120, 260, 150, hWnd, NULL, hInstance, NULL);

	TextLoad = CreateWindowEx(WS_EX_CLIENTEDGE ,"EDIT", pngfile, WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_BORDER, 20, 10, 150, 20, hWnd, NULL, NULL, NULL);
	TextSave = CreateWindowEx(WS_EX_CLIENTEDGE ,"EDIT", cfile, WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_BORDER, 20, 40, 150, 20, hWnd, NULL, NULL, NULL);

	/*Button's*/
	//hButton = CreateWindow("button", "Repair", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 50, 100, 30, hWnd, (HMENU)ButtonRepair, hInstance, 0);
	cButton = CreateWindow("button", "Create", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200, 75, 100, 30, hWnd, (HMENU)ButtonCreate, hInstance, 0);


	//ListBox (select your save here)
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 200, 10, 150, 60, hWnd, (HMENU)listbox, NULL, NULL);
	
	/*Checkbox*/
	IsHexadecimal = CreateWindow("button" , "Hexadecimal Array", BS_CHECKBOX | WS_VISIBLE | WS_CHILD |BST_CHECKED, 20, 85, 165, 25, hWnd, (HMENU)IsHexadecimalCheckBox, hInstance, NULL);

	

	SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)"Default");
	SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)"NTR Plugin");
	SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)"CTRPluginFramework");
	SendMessage(hList, LB_SELECTSTRING, NULL, (LPARAM)"Default");
	SetMenu(hWnd, hMenu);

	//Main loop
	while (true) {
		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			
			return result;
		}
	}
}