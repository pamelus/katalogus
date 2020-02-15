#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <Shlwapi.h>
#include "kontrolki.h"

void ConvertTo(char * pszString)
{
    for(int i = 0; i < lstrlen(pszString); i++)
        if(pszString[i] == '|') pszString[i] = '\n';
}

void ConvertFrom(char * pszString)
{
    for(int i = 0; i < lstrlen(pszString); i++)
        if(pszString[i] == '\n') pszString[i] = '|';
}

BOOL CALLBACK AboutProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "A");
            SendMessage(GetDlgItem(hwnd, 101), STM_SETICON, (WPARAM)hIcon, 0);
            DestroyIcon(hIcon);
            
            return FALSE;
        }
        case WM_COMMAND:
        {
            if(LOWORD(wParam) == 100) EndDialog(hwnd, 0);
        }
    }
    return FALSE;
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hThis = NULL;
    HMENU hMenu = NULL;
    
    static int iPos = 0;
    static int iMaxPos = 0;
    static int * iText = NULL;
    
    const int iMaxText = 214;
    
    static char pszText[iMaxText][256];
    static char pszFile[1024] = "";
    
    switch(message)
    {
        case WM_INITDIALOG:
        {
            RECT rect; GetWindowRect(hwnd, &rect);
            int cy = rect.bottom - rect.top;
            int cx = rect.right - rect.left;
            int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
            int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;
            MoveWindow(hwnd, x, y, cx, cy, false);
            
            for(int i = 0; i < iMaxText; i++)
            {
                lstrcpy(pszText[i], "");
            }
            
            hThis = GetModuleHandle(NULL);
            hMenu = LoadMenu(hThis, "MAINMENU");
            
            SetMenu(hwnd, hMenu);
            
            HICON hIcon = LoadIcon(hThis, "A");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            HWND hTab = GetDlgItem(hwnd, 100);
            TCITEM tci; ZeroMemory(&tci, sizeof(TCITEM));
            tci.mask = TCIF_TEXT;
            
            tci.pszText = "Menu g³ówne";
            TabCtrl_InsertItem(hTab, 0, &tci);
            
            tci.pszText = "Menu kontekstowe";
            TabCtrl_InsertItem(hTab, 1, &tci);
            
            tci.pszText = "Okna dialogowe";
            TabCtrl_InsertItem(hTab, 2, &tci);
            
            tci.pszText = "Nag³ówki kolumn";
            TabCtrl_InsertItem(hTab, 3, &tci);
            
            tci.pszText = "Tool tipy";
            TabCtrl_InsertItem(hTab, 4, &tci);
            
            tci.pszText = "B³êdy";
            TabCtrl_InsertItem(hTab, 5, &tci);
            
            tci.pszText = "Inne";
            TabCtrl_InsertItem(hTab, 6, &tci);
            
            tci.pszText = "O autorze";
            TabCtrl_InsertItem(hTab, 7, &tci);
            
            NMHDR nmhdr;
            nmhdr.hwndFrom = hTab;
            nmhdr.code = TCN_SELCHANGE;
            nmhdr.idFrom = 100;
            
            SendMessage(hwnd, WM_NOTIFY, 0, (LPARAM)&nmhdr);
            
            return FALSE;
        }
        case WM_DESTROY:
        {
            SetMenu(hwnd, NULL);
            DestroyMenu(hMenu);
            
            if(iText) delete [] iText;
            
            PostQuitMessage(0);
            return TRUE;
        }
        case WM_CLOSE:
        {   
            if(MessageBox(hwnd, "Czy na pewno chcesz zakoñczyæ dzia³anie programu?\nWszelkie nie zapisane dane zostan¹ utracone.", "Koñczenie pracy programu", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
            return TRUE;
        }
        case WM_CTLCOLORSTATIC:
        {
            SetBkMode((HDC)wParam, TRANSPARENT);
            HBRUSH hBrush = NULL;
            return (int)hBrush;
        }
        case WM_NOTIFY:
        {
            NMHDR * nmhdr = (LPNMHDR)lParam;
            switch(nmhdr->code)
            {
                case TCN_SELCHANGE:
                {
                    if(iText) GetDlgItemText(hwnd, 201, pszText[iText[iPos]], 256);
                    
                    EnableWindow(GetDlgItem(hwnd, 301), false);
                    EnableWindow(GetDlgItem(hwnd, 302), true);
                    
                    char pszOpis[256];
                    if(iText) delete [] iText; iText = NULL;
                    iPos = 0;
                    switch(TabCtrl_GetCurSel(nmhdr->hwndFrom))
                    {
                        case 0:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla elementów g³ównego menu.");
                            iMaxPos = 37;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                            {
                                iText[i] = i;
                                if(i > 25) iText[i]--;
                            }    
                            iText[25] = 51;
                            break;
                        }    
                        case 1:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla elementów menu kontekstowego."); 
                            iMaxPos = 13;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = 37 + i;
                            break;
                        }    
                        case 2:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla okien dialogowych.");
                            iMaxPos = 133;
                            iText = new int[iMaxPos + 1];
                            
                            int iTable[] = { 52 + 6, 52 + 0, 52 + 1, 52 + 2, 52 + 3, 52 + 4, 52 + 5, 52 + 7, 52 + 8, 52 + 9, 52 + 127, 52 + 129, 52 + 10, 52 + 128, 52 + 11, 52 + 130, 52 + 131, 52 + 132, 52 + 12, 52 + 13, 52 + 14, 52 + 15, 52 + 28, 52 + 16, 52 + 17, 52 + 18, 52 + 19, 52 + 20, 52 + 21, 52 + 22, 52 + 29, 52 + 23, 52 + 133, 52 + 24, 52 + 25, 52 + 26, 52 + 27, 52 + 30, 52 + 74, 52 + 75, 52 + 76, 52 + 77, 52 + 78, 52 + 31, 52 + 79, 52 + 80, 52 + 81, 52 + 82, 52 + 83, 52 + 84, 52 + 85, 52 + 86, 52 + 87, 52 + 88, 52 + 89, 52 + 90, 52 + 91, 52 + 92, 52 + 93, 52 + 94, 52 + 95, 52 + 96, 52 + 97, 52 + 98, 52 + 99, 52 + 100, 52 + 32, 52 + 33, 52 + 34, 52 + 35, 52 + 36, 52 + 37, 52 + 38, 52 + 39, 52 + 40, 52 + 41, 52 + 42, 52 + 43, 52 + 44, 52 + 45, 52 + 46, 52 + 47, 52 + 48, 52 + 49, 52 + 50, 52 + 51, 52 + 52, 52 + 53, 52 + 54, 52 + 55, 52 + 56, 52 + 57, 52 + 58, 52 + 59, 52 + 60, 52 + 61, 52 + 62, 52 + 63, 52 + 64, 52 + 65, 52 + 66, 52 + 67, 52 + 68, 52 + 69, 52 + 70, 52 + 71, 52 + 72, 52 + 73, 52 + 101, 52 + 102, 52 + 105, 52 + 106, 52 + 107, 52 + 108, 52 + 109, 52 + 110, 52 + 111, 52 + 112, 52 + 113, 52 + 114, 52 + 103, 52 + 115, 52 + 116, 52 + 117, 52 + 118, 52 + 119, 52 + 104, 52 + 120, 52 + 121, 52 + 122, 52 + 123, 52 + 124, 52 + 125, 52 + 126 };
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = iTable[i];
                            
                            break;
                        }
                        case 3:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla nag³ówków kolumn.");
                            iMaxPos = 4;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = i + 52 + 134;
                            
                            break;
                        }
                        case 4:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla tool tipów.");
                            iMaxPos = 9;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = iMaxText - 10 + i;
                            break;
                        }    
                        case 5:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie b³êdów.");
                            iMaxPos = 4;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = iMaxText - 21 + i;
                            break;
                        }
                        case 6:
                        {
                            lstrcpy(pszOpis, "Podaj t³umaczenie dla innych elementów programu.");
                            iMaxPos = 5;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = iMaxText - 16 + i;
                            break;
                        }
                        case 7:
                        {
                            lstrcpy(pszOpis, "Podaj swoje nazwisko i adres e-mail");
                            iMaxPos = 1;
                            iText = new int[iMaxPos + 1];
                            for(int i = 0; i < iMaxPos + 1; i++)
                                iText[i] = iMaxText - 2 + i;
                            break;
                        }
                    }
                    SetDlgItemText(hwnd, 200, pszOpis);
                    SetDlgItemText(hwnd, 201, pszText[iText[iPos]]);
                    
                    wsprintf(pszOpis, "%i z %i", iPos + 1, iMaxPos + 1);
                    SetDlgItemText(hwnd, 202, pszOpis);
                    
                    SetFocus(GetDlgItem(hwnd, 201));
                    return TRUE;
                }
            }
            break;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case 301:
                {
                    GetDlgItemText(hwnd, 201, pszText[iText[iPos]], 256);
                    iPos--;
                    SetDlgItemText(hwnd, 201, pszText[iText[iPos]]);
                    
                    char pszOpis[16];
                    wsprintf(pszOpis, "%i z %i", iPos + 1, iMaxPos + 1);
                    SetDlgItemText(hwnd, 202, pszOpis);
                    
                    EnableWindow(GetDlgItem(hwnd, 302), true);
                    if(iPos == 0) EnableWindow(GetDlgItem(hwnd, 301), false);
                    SetFocus(GetDlgItem(hwnd, 201));
                    return TRUE;
                }
                case 302:
                {
                    GetDlgItemText(hwnd, 201, pszText[iText[iPos]], 256);
                    iPos++;
                    SetDlgItemText(hwnd, 201, pszText[iText[iPos]]);
                    
                    char pszOpis[16];
                    wsprintf(pszOpis, "%i z %i", iPos + 1, iMaxPos + 1);
                    SetDlgItemText(hwnd, 202, pszOpis);
                    
                    EnableWindow(GetDlgItem(hwnd, 301), true);
                    if(iPos == iMaxPos) EnableWindow(GetDlgItem(hwnd, 302), false);
                    SetFocus(GetDlgItem(hwnd, 201));
                    return TRUE;
                }
                case IDC_NOWY:
                {
                    for(int i = 0; i < iMaxText; i++)
                        lstrcpy(pszText[i], "");
                    
                    lstrcpy(pszFile, "");    
                    
                    SetDlgItemText(hwnd, 201, pszText[iText[iPos]]);
                    return TRUE;
                }
                case IDC_WCZYTAJ:
                {
                    OPENFILENAME ofn;
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hwnd;
                    ofn.hInstance = hThis;
                    ofn.lpstrFilter = "Pliki jêzykowe programu Katalogus (*.kln)\0*.kln\0Wszystkie pliki (*.*)\0*.*\0\0";
                    ofn.lpstrCustomFilter = NULL;
                    ofn.nMaxCustFilter = 0;
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFile = pszFile;
                    ofn.nMaxFile = 1024;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.lCustData = 0;
                    ofn.lpstrTitle = "Wczytywanie pliku jêzykowego";
                    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                        
                    if(GetOpenFileName(&ofn))
                    {
                        FILE * pFile = fopen(pszFile, "rb");
                        if(!pFile)
                        {
                            char pszMessage[2048];
                            wsprintf(pszMessage, "Nie mo¿na otworzyæ pliku %s.", pszFile);
                            MessageBox(hwnd, pszMessage, "B³¹d", MB_ICONERROR | MB_OK);
                            return TRUE;
                        }
                        
                        fread(pszText, 256, iMaxText, pFile);
                        fclose(pFile);
                        
                        for(int i = 0; i < iMaxText; i++)
                            ConvertFrom(pszText[i]);
                        
                        SetDlgItemText(hwnd, 201, pszText[iText[iPos]]);
                    }
                    return TRUE;
                }
                case IDC_ZAPISZ:
                {
                    if(pszFile[0] == 0)
                    {
                        SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_ZAPISZJAKO, 0), 0);
                        return TRUE;
                    }
                    
                    FILE * pFile = fopen(pszFile, "wb");
                    if(!pFile)
                    {
                        char pszMessage[2048];
                        wsprintf(pszMessage, "Nie mo¿na otworzyæ pliku %s.", pszFile);
                        MessageBox(hwnd, pszMessage, "B³¹d", MB_ICONERROR | MB_OK);
                        return TRUE;
                    }
                    
                    GetDlgItemText(hwnd, 201, pszText[iText[iPos]], 256);
                    
                    for(int i = 0; i < iMaxText; i++)
                        ConvertTo(pszText[i]);
                    
                    fwrite(pszText, 256, iMaxText, pFile);
                    
                    for(int i = 0; i < iMaxText; i++)
                        ConvertFrom(pszText[i]);
                    
                    fclose(pFile);
                        
                    return TRUE;
                }
                case IDC_ZAPISZJAKO:
                {
                    OPENFILENAME ofn;
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hwnd;
                    ofn.hInstance = hThis;
                    ofn.lpstrFilter = "Pliki jêzykowe programu Katalogus (*.kln)\0*.kln\0Wszystkie pliki (*.*)\0*.*\0\0";
                    ofn.lpstrCustomFilter = NULL;
                    ofn.nMaxCustFilter = 0;
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFile = pszFile;
                    ofn.nMaxFile = 1024;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.lpstrDefExt = "kln";
                    ofn.lCustData = 0;
                    ofn.lpstrTitle = "Zapisywanie pliku jêzykowego";
                    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                    
                    if(GetSaveFileName(&ofn))
                    {
                        SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_ZAPISZ, 0), 0);
                    }
                    return TRUE;
                }
                case IDC_KONIEC:
                {
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    return TRUE;
                }
                case IDC_POMOC:
                {
                    char pszFile[1024]; GetModuleFileName(hThis, pszFile, 1024);
                    PathRemoveFileSpec(pszFile);
                    PathAppend(pszFile, "manager.hlp");
                    
                    ShellExecute(HWND_DESKTOP, "open", pszFile, NULL, NULL, SW_SHOW);
                    return TRUE;
                }
                case IDC_OPROGRAMIE:
                {
                    DialogBox(hThis, "OPROGRAMIE", hwnd, AboutProc);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrevInstance, char * pszFile, int iShow)
{
    InitCommonControls();
    
    HWND hDialog = CreateDialog(hThis, "MAIN_DIALOG", HWND_DESKTOP, DialogProc);
    ShowWindow(hDialog, iShow);
    
    HACCEL hAccel = LoadAccelerators(hThis, "ACCELS");
    
    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0))
    {
        if(!TranslateAccelerator(hDialog, hAccel, &messages))
        {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
    }
    DestroyAcceleratorTable(hAccel);
    
    return 0;
}
