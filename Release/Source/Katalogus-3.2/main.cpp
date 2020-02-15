//#define _DEBUG_

#define _WIN32_IE               0x0500
#define TB_SETIMAGELIST         (WM_USER + 48)
#define HKEY_MYKEY              "Software\\XamaxSoftware\\Katalogus"

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include <Shlwapi.h>
#include <LibId3.h>
#include <LibReg.h>
#include "assert.h"
#include "Bledy.h"
#include "Kontrolki.h"
#include "Opcje.h"
#include "Jezyk.h"
#include "Katalog.h"
#include "Interfejs.h"
#include "Recent.h"

// zmienne globalne
char pszProgramName[] = "Katalogus 3.2";
HWND hOkna = NULL;

// deklaracje funkcji
DWORD WINAPI ScanThread(LPVOID lParam);
DWORD WINAPI SaveThread(LPVOID lParam);
DWORD WINAPI LoadThread(LPVOID lParam);
DWORD WINAPI SortThread(LPVOID lParam);
DWORD WINAPI SearchThread(LPVOID lParam);
DWORD WINAPI ReportThread(LPVOID lParam);

LRESULT CALLBACK WinMainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainDbProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TipProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListViewProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ItemProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ScanItemProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ScanProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK StatsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Wizard0Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Wizard1Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Wizard2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Wizard3Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ResultProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AccelProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PsswdProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Options1Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Options2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Options3Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int ListView_AddItems(HWND hWidok, int iImage, DANE * dane, int liczba);

bool EnableSafetyBox(HWND hwnd, bool iEnable);
bool EnableMainMenu(bool iEnable);
bool EnablePositionMenu(HMENU hContext, bool iEnable);

void DodajDoRaportu(Katalog * report, const int * iDane, const DANE * dane1, const DANE * dane2, const DANE * dane3);
bool CreateFilter(char * pszFilter, int iItem);
bool SkanujKatalog(char * pszFile, SCANTAG * scn);
bool PetlaSterujaca(MSG * message);
void ObslugaBledu();

bool FloatToString(int iLiczba1, int iLiczba2, char * pszString, int iSize);
char * AccelAlloc(const char * pText);

inline int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) { return lstrcmp((char *)lParam1, (char *)lParam2); }

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrevInstance, char * pszFile, int iShow)
{
    HANDLE hMutex = CreateMutex(NULL, true, "Xamax Software Katalogus");
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        HWND hwnd = FindWindow("katalogus", NULL);
        SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
        SetForegroundWindow(hwnd);
        
        if(pszFile[0] != 0)
        {
            RKey rKey;
            rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_WRITE);
            rKey.ZapiszRekordC("ReadFile", pszFile);
            rKey.Zamknij();
            
            SendMessage(hwnd, WM_READFILE, 0, 0);
        }
        return 0;
    }
    
    INITCOMMONCONTROLSEX init;
    init.dwSize = sizeof(INITCOMMONCONTROLSEX);
    init.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES;
    
    InitCommonControlsEx(&init);
    LoadLibrary("RICHED32.DLL");
    
    WNDCLASSEX wincl1;
    wincl1.hInstance = hThis;
    wincl1.lpszClassName = "katalogus";
    wincl1.lpfnWndProc = WinMainProc;
    wincl1.style = CS_DBLCLKS;
    wincl1.cbSize = sizeof(WNDCLASSEX);
    wincl1.hIcon = LoadIcon(hThis, "A");
    wincl1.hIconSm = LoadIcon(hThis, "A");
    wincl1.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl1.lpszMenuName = NULL;
    wincl1.cbClsExtra = 0;
    wincl1.cbWndExtra = 0;
    wincl1.hbrBackground = (HBRUSH)COLOR_WINDOW;
    
    WNDCLASSEX wincl2;
    wincl2.hInstance = hThis;
    wincl2.lpszClassName = "dbwindow";
    wincl2.lpfnWndProc = MainDbProc;
    wincl2.style = CS_DBLCLKS;
    wincl2.cbSize = sizeof(WNDCLASSEX);
    wincl2.hIcon = LoadIcon(hThis, "B");
    wincl2.hIconSm = LoadIcon(hThis, "B");
    wincl2.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl2.lpszMenuName = NULL;
    wincl2.cbClsExtra = 0;
    wincl2.cbWndExtra = 0;
    wincl2.hbrBackground = (HBRUSH)COLOR_WINDOW;
    
    WNDCLASSEX wincl3;
    wincl3.hInstance = hThis;
    wincl3.lpszClassName = "tipwindow";
    wincl3.lpfnWndProc = TipProc;
    wincl3.style = CS_DBLCLKS;
    wincl3.cbSize = sizeof(WNDCLASSEX);
    wincl3.hIcon = NULL;
    wincl3.hIconSm = NULL;
    wincl3.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl3.lpszMenuName = NULL;
    wincl3.cbClsExtra = 0;
    wincl3.cbWndExtra = 0;
    wincl3.hbrBackground = (HBRUSH)COLOR_WINDOW;
    
    if(!RegisterClassEx(&wincl1) || !RegisterClassEx(&wincl2) || !RegisterClassEx(&wincl3))
        return 0;
    
    hOkna = CreateWindowEx(0, "katalogus", pszProgramName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 500, HWND_DESKTOP, 0, hThis, NULL);
    if(!hOkna) return 0;
    
    EnableMainMenu(false);
    ShowWindow(hOkna, iShow);
    
    if(pszFile[0] != 0)
    {
        char pszFile2[1024];
        lstrcpyn(pszFile2, &pszFile[1], lstrlen(pszFile) - 1);
        SetMessageExtraInfo((LPARAM) pszFile2);
        SendMessage(hOkna, WM_COMMAND, MAKELONG(IDC_WCZYTAJ, 0), 0);
        SetMessageExtraInfo(0);
    }
    
    MSG messages;
    HACCEL hAccel = LoadAccelerators(hThis, "ACCELS");
    while (GetMessage (&messages, NULL, 0, 0))
    {
        if(!TranslateAccelerator(hOkna, hAccel, &messages))
        {
            if(!TranslateMDISysAccel(GetDlgItem(hOkna, ID_MDI), &messages))
            {
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
        }
    }
    DestroyAcceleratorTable(hAccel);
    return 0;
}

bool PetlaSterujaca(MSG * msg)
{
    bool isMessage = PeekMessage(msg, 0, 0, 0, PM_REMOVE);
    if(isMessage)
    {
        TranslateMessage(msg);
        DispatchMessage(msg);
    }
    return isMessage;
}

bool FloatToString(int iLiczba1, int iLiczba2, char * pszString, int iSize)
{
    if(iLiczba2 == 0)
    {
        lstrcpy(pszString, "0");
        return false;
    }
    
    int iWynik = (iLiczba1 * 100) / iLiczba2;
    
    wsprintf(pszString, "%i", iWynik);
    
    int iLen = lstrlen(pszString);
    pszString[iLen + 1] = pszString[iLen];
    pszString[iLen] = pszString[iLen - 1];
    pszString[iLen - 1] = pszString[iLen - 2];
    pszString[iLen - 2] = ',';
    
    return true;
}

char * AccelAlloc(const char * pszText)
{
    char * pszTemp = new char[lstrlen(pszText) + 2];
    lstrcpy(pszTemp, "\t");
    lstrcat(pszTemp, pszText);
    return pszTemp;
}

bool EnableMainMenu(bool iEnable)
{
    DWORD dwStyle = MF_BYCOMMAND | (iEnable) ? MF_ENABLED : MF_GRAYED;
    
    HMENU hMenu = GetMenu(hOkna);
    
    EnableMenuItem(GetSubMenu(hMenu, 0), IDC_ZAPISZ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 0), IDC_ZAPISZJAKO, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 0), IDC_ZAMKNIJ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_OTWORZ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_WSTECZ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_DODAJ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_EDYTUJ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_USUN, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_GORA, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_DOL, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_SZEREGUJNAZWA, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_SZEREGUJNOSNIK, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_SZEREGUJOPIS, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 3), IDC_RAPORT, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 3), IDC_STATYSTYKA, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 2), IDC_SZUKAJNAZWA, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 2), IDC_SZUKAJNOSNIK, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 3), IDC_SKANUJ, dwStyle);
    
    return true;
}

bool EnablePositionMenu(HMENU hContext, bool iEnable)
{
    DWORD dwStyle = MF_BYCOMMAND | (iEnable) ? MF_ENABLED : MF_GRAYED;
    
    HMENU hMenu = GetMenu(hOkna);
    
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_OTWORZ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_EDYTUJ, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_USUN, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_GORA, dwStyle);
    EnableMenuItem(GetSubMenu(hMenu, 1), IDC_DOL, dwStyle);
    
    EnableMenuItem(hContext, IDC_OTWORZ, dwStyle);
    EnableMenuItem(hContext, IDC_EDYTUJ, dwStyle);
    EnableMenuItem(hContext, IDC_USUN, dwStyle);
    EnableMenuItem(hContext, IDC_GORA, dwStyle);
    EnableMenuItem(hContext, IDC_DOL, dwStyle);
    
    return true;
}

bool CreateFilter(char * pszFilter, int iItem)
{
    switch(iItem)
    {
        case 1:
            lstrcpy(pszFilter, Jezyk::pszDialog[128]);
            lstrcat(pszFilter, " (*.html; *.htm)");
                    
            pszFilter += lstrlen(pszFilter) + 1;
            lstrcpy(pszFilter, "*.html;*.htm");
            break;
        default:
            lstrcpy(pszFilter, Jezyk::pszDialog[10]);
            lstrcat(pszFilter, " (*.kdb)");
                    
            pszFilter += lstrlen(pszFilter) + 1;
            lstrcpy(pszFilter, "*.kdb");
            break;
    }        
                    
    pszFilter += lstrlen(pszFilter) + 1;
    lstrcpy(pszFilter, Jezyk::pszDialog[11]);
    lstrcat(pszFilter, " (*.*)");
                    
    pszFilter += lstrlen(pszFilter) + 1;
    lstrcpy(pszFilter, "*.*");
                    
    pszFilter += lstrlen(pszFilter) + 1;
    *pszFilter = 0;
    
    return true;
}

LRESULT CALLBACK WinMainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hThis = NULL;
    static HWND hTool = NULL;
    static HWND hStatus = NULL;
    static HWND hMdi = NULL;
    static HMENU hMain = NULL;
    switch(message)
    {
        case WM_CREATE:
        {
            CREATESTRUCT * cs = (LPCREATESTRUCT)lParam;
            hThis = cs->hInstance;
            
            char pszLanguage[1024] = "";
            
            RKey rKey;
            rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_READ);
            rKey.WczytajRekordC("pszLanguage", pszLanguage, 1024);
            rKey.Zamknij();
            
            if(!Jezyk::Inicjuj(pszLanguage))
            {
                ObslugaBledu();
                Jezyk::Domyslne(hThis);
            }
                
            hMain = CreateMainMenu(hThis);
            if(!hMain)
            {
                ObslugaBledu();
                return -1;
            }
            
            Opcje::Wczytaj();
            
            Recent::Inicjuj();
            Recent::UaktualnijMenu(GetSubMenu(hMain, 0));
            
            SetMenu(hwnd, hMain);
                
            RECT rect; GetClientRect(hwnd, &rect);
                
            hTool = CreateToolWindow(hwnd, hThis);
            if(!hTool)
            {
                ObslugaBledu();
                return -1;
            }
            
            RECT rcTool; GetWindowRect(hTool, &rcTool);
                
            hStatus = CreateStatusWindow(hwnd, hThis);
            if(!hStatus)
            {
                ObslugaBledu();
                return -1;
            }
            
            RECT rcStatus; GetWindowRect(hStatus, &rcStatus);
                
            hMdi = CreateMdiWindow(rect.left, rect.top + (rcTool.bottom - rcTool.top), rect.right - rect.left, rect.bottom - rect.top - (rcTool.bottom - rcTool.top) - (rcStatus.bottom - rcStatus.top), hwnd, hThis);
            if(!hMdi)
            {
                ObslugaBledu();
                return -1;
            }
            
            SetTimer(hwnd, IDTM_TIP, 1100, NULL);
            
            if(Opcje::IsSizeEnabled())
            {
                RECT rect; GetWindowRect(hwnd, &rect);
                
                RKey rKey;
                rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_READ);
                rKey.WczytajRekordD("rcWindow", &rect, sizeof(RECT));
                rKey.Zamknij();
                
                if(HIWORD(rect.left) == 1234)
                {
                    SendMessage(hwnd, WM_SYSCOMMAND, MAKELONG(SC_MAXIMIZE, 0), 0);
                }
                else
                {
                    MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
                }
            }
            
            return 0;
        }
        case WM_CLOSE:
        {
            HWND hActive = NULL;
            while(hActive = (HWND)SendMessage(hMdi, WM_MDIGETACTIVE, 0, 0))
                if(SendMessage(hActive, WM_CLOSE, 0, 0) == -1) return -1;
                
            break;
        }
        case WM_DESTROY:
        {
            KillTimer(hwnd, IDTM_TIP);
            
            if(Opcje::IsSizeEnabled())
            {
                RECT rect; GetWindowRect(hwnd, &rect);
                if(GetWindowLong(hwnd, GWL_STYLE) & WS_MAXIMIZE)
                {
                    SendMessage(hwnd, WM_SYSCOMMAND, MAKELONG(SC_RESTORE, 0), 0);                    
                    rect.left = MAKELONG((UINT)rect.left, 1234);
                }
                
                if(GetWindowLong(hwnd, GWL_STYLE) & WS_MINIMIZE)
                {
                    SendMessage(hwnd, WM_SYSCOMMAND, MAKELONG(SC_RESTORE, 0), 0);
                    GetWindowRect(hwnd, &rect);
                }
                
                RKey rKey;
                rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_WRITE);
                rKey.ZapiszRekordD("rcWindow", &rect, sizeof(RECT));
                rKey.Zamknij();
            }
            
            SetMenu(hwnd, NULL);
            DestroyMenu(hMain);
            hMain = NULL;
            
            Recent::Zamknij();
            Opcje::Zapisz();
            Jezyk::Zamknij();
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZING:
        {
            RECT * rect = (LPRECT)lParam;
            if((rect->bottom - rect->top) < 400) rect->bottom = rect->top + 400;
            if((rect->right - rect->left) < 600) rect->right = rect->left + 600;
            return 0;
        }
        case WM_SIZE:
        {
            MoveWindow(hStatus, 0, 0, 0, 0, true);
            SendMessage(hTool, TB_AUTOSIZE, 0, 0);
            
            RECT rect; GetClientRect(hwnd, &rect);
            RECT rcTool; GetWindowRect(hTool, &rcTool);
            RECT rcStatus; GetWindowRect(hStatus, &rcStatus);
            
            MoveWindow(hMdi, rect.left, rect.top + (rcTool.bottom - rcTool.top), rect.right - rect.left, rect.bottom - rect.top - (rcTool.bottom - rcTool.top) - (rcStatus.bottom - rcStatus.top), true);
            
            return 0;
        }
        case WM_SETFOCUS:
        {
            SetFocus(hMdi);            
            return 0;
        }
        case WM_MENUSELECT:
        {
            HMENU hSelected = (HMENU)lParam;
            UINT uFlags = (UINT)HIWORD(wParam);
            
            HWND hActive = (HWND)SendMessage(hMdi, WM_MDIGETACTIVE, 0, 0);
            
            if(hSelected == NULL && uFlags == 0xFFFF)
            {
                SendMessage(hActive, WM_TIPENABLE, 0, 0);
            }
            else
            {
                SendMessage(hActive, WM_TIPDISABLE, 0, 0);
            }
            return 0;
        }
        case WM_READFILE:
        {
            char pszFile[1024];
            
            RKey rKey;
            rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_READ);
            rKey.WczytajRekordC("ReadFile", pszFile, 1024);
            rKey.Zamknij();
            
            char pszFile2[1024];
            lstrcpyn(pszFile2, &pszFile[1], lstrlen(pszFile) - 1);
            SetMessageExtraInfo((LPARAM)pszFile2);
            SendMessage(hOkna, WM_COMMAND, MAKELONG(IDC_WCZYTAJ, 0), 0);
            SetMessageExtraInfo(0);

            return 0;
        }
        case WM_TIPDESTROY:
        {
            HWND hClient = (HWND)SendMessage(hMdi, WM_MDIGETACTIVE, 0, 0);
            SendMessage(hClient, message, wParam, lParam);
            return 0;
        }
        case WM_TIMER:
        {
            switch(LOWORD(wParam))
            {
                case IDTM_TIP:
                {
                    static POINT pp = { 0, 0 };
                    
                    POINT p; GetCursorPos(&p);
                    if(p.x == pp.x && p.y == pp.y)
                    {
                        HWND hClient = (HWND)SendMessage(hMdi, WM_MDIGETACTIVE, 0, 0);
                        SendMessage(hClient, WM_TIPCREATE, 0, 0);
                    }
                    else
                    {
                        pp.x = p.x;
                        pp.y = p.y;
                    }    
                    return 0;
                }
            }
            break;
        }
        case WM_NOTIFY:
        {
            NMHDR * nmhdr = (LPNMHDR)lParam;
            switch(nmhdr->code)
            {
                case TTN_GETDISPINFO:
                {
                    LPNMTTDISPINFO tdi = (LPTOOLTIPTEXT)lParam;
                    switch((int)wParam)
                    {
                        case IDC_NOWA:
                            tdi->lpszText = Jezyk::pszInne[6];
                            break;
                        case IDC_WCZYTAJ:
                            tdi->lpszText = Jezyk::pszInne[7];
                            break;
                        case IDC_ZAPISZ:
                            tdi->lpszText = Jezyk::pszInne[8];
                            break;
                        case IDC_DODAJ:
                            tdi->lpszText = Jezyk::pszInne[9];
                            break;
                        case IDC_EDYTUJ:
                            tdi->lpszText = Jezyk::pszInne[10];
                            break;
                        case IDC_USUN:
                            tdi->lpszText = Jezyk::pszInne[11];
                            break;
                        case IDC_OTWORZ:
                            tdi->lpszText = Jezyk::pszInne[12];
                            break;
                        case IDC_WSTECZ:
                            tdi->lpszText = Jezyk::pszInne[13];
                            break;
                        case IDC_GORA:
                            tdi->lpszText = Jezyk::pszInne[14];
                            break;
                        case IDC_DOL:
                            tdi->lpszText = Jezyk::pszInne[15];
                            break;
                    }
                    return 0;
                }
            }
            break;
        }
        case WM_COMMAND:
        {
            HWND hClient = (HWND)SendMessage(hMdi, WM_MDIGETACTIVE, 0, 0);
            switch(LOWORD(wParam))
            {
                case IDC_NOWA:
                {
                    SendMessage(hClient, WM_TIPDISABLE, 0, 0);
                    
                    MDICREATESTRUCT mcs;
                    mcs.szClass = "dbwindow";
                    mcs.szTitle = Jezyk::pszDialog[6];
                    mcs.hOwner = hThis;
                    mcs.x = mcs.cx = CW_USEDEFAULT;
                    mcs.y = mcs.cy = CW_USEDEFAULT;
                    mcs.style = MDIS_ALLCHILDSTYLES;
                    mcs.lParam = 0;
                
                    SendMessage(hMdi, WM_MDICREATE, 0, (LONG)&mcs);                    
                    SendMessage(hClient, WM_TIPENABLE, 0, 0);
                    
                    return 0;
                }
                case IDC_RECENT1:
                case IDC_RECENT2:
                case IDC_RECENT3:
                case IDC_RECENT4:
                case IDC_RECENT5:
                {
                    char pszFile[1024];
                    Recent::PobierzPlik(LOWORD(wParam) - IDC_RECENT1, pszFile, 1024);
                    SetMessageExtraInfo((LPARAM)pszFile);
                    SendMessage(hOkna, WM_COMMAND, MAKELONG(IDC_WCZYTAJ, 0), 0);
                    SetMessageExtraInfo(0);
                    
                    return 0;
                }
                case IDC_WCZYTAJ:
                {
                    SendMessage(hClient, WM_TIPDISABLE, 0, 0);
                    
                    MDICREATESTRUCT mcs;
                    mcs.szClass = "dbwindow";
                    mcs.szTitle = Jezyk::pszDialog[6];
                    mcs.hOwner = hThis;
                    mcs.x = mcs.cx = CW_USEDEFAULT;
                    mcs.y = mcs.cy = CW_USEDEFAULT;
                    mcs.style = MDIS_ALLCHILDSTYLES;
                    mcs.lParam = (LPARAM)"0";
                    
                    char * pszFile = (char *)GetMessageExtraInfo();
                    if(pszFile)
                    {
                        mcs.lParam = (LPARAM)pszFile;
                    }
                    
                    SendMessage(hMdi, WM_MDICREATE, 0, (LONG)&mcs);
                    SendMessage(hClient, WM_TIPENABLE, 0, 0);
                    
                    return 0;
                }
                case IDC_KONIEC:
                {
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    return 0;
                }
                case IDC_OPCJE:
                {
                    SendMessage(hClient, WM_TIPDISABLE, 0, 0);
                    
                    HPROPSHEETPAGE hPages[3];
                    
                    PROPSHEETPAGE page;
                    page.dwSize = sizeof(PROPSHEETPAGE);
                    page.dwFlags = PSP_USEICONID | PSP_USETITLE;
                    page.hInstance = hThis;
                    page.lParam = 0;
                    
                    page.pszTemplate = "OPTIONS_MAIN_DIALOG";
                    page.pszIcon = "OPTIONS_MAIN_ICON";
                    page.pszTitle = Jezyk::pszDialog[102];
                    page.pfnDlgProc = Options1Proc;
                    hPages[0] = CreatePropertySheetPage(&page);
                    
                    page.pszTemplate = "OPTIONS_VISUAL_DIALOG";
                    page.pszIcon = "OPTIONS_VISUAL_ICON";
                    page.pszTitle = Jezyk::pszDialog[103];
                    page.pfnDlgProc = Options2Proc;
                    hPages[1] = CreatePropertySheetPage(&page);
                    
                    page.pszTemplate = "OPTIONS_SAFETY_DIALOG";
                    page.pszIcon = "OPTIONS_SAFETY_ICON";
                    page.pszTitle = Jezyk::pszDialog[104];
                    page.pfnDlgProc = Options3Proc;
                    hPages[2] = CreatePropertySheetPage(&page);
                    
                    PROPSHEETHEADER sheet;
                    sheet.dwSize = sizeof(PROPSHEETHEADER);
                    sheet.dwFlags = PSH_NOAPPLYNOW | PSH_USEICONID;
                    sheet.hwndParent = hwnd;
                    sheet.hInstance = hThis;
                    sheet.pszIcon = "OPTIONS_ICON";
                    sheet.pszCaption = Jezyk::pszDialog[101];
                    sheet.nPages = 3;
                    sheet.pStartPage = 0;
                    sheet.phpage = hPages;
                    
                    Options1Proc(NULL, WM_SETINFO, 0, 0);
                    Options2Proc(NULL, WM_SETINFO, 0, 0);
                    Options3Proc(NULL, WM_SETINFO, 0, 0);
                    
                    if(PropertySheet(&sheet) == IDOK)
                    {
                        Options1Proc(NULL, WM_GETINFO, 0, 0);
                        Options2Proc(NULL, WM_GETINFO, 0, 0);
                        Options3Proc(NULL, WM_GETINFO, 0, 0);
                        
                        HWND hChild = NULL;
                        for(int i = 0; hChild = GetDlgItem(hMdi, ID_WINDOW + i); i++)
                        {
                            HWND hWidok = GetDlgItem(hChild, ID_WIDOK);
                            
                            ListView_SetTextColor(hWidok, Opcje::GetTextColor1());
                            ListView_SetTextBkColor(hWidok, Opcje::GetBkColor1());
                            ListView_SetBkColor(hWidok, Opcje::GetBkColor1());
                            
                            LOGFONT lFont; Opcje::GetFont1(&lFont);
                            SendMessage(hWidok, WM_SETFONT, (WPARAM)CreateFontIndirect(&lFont), TRUE);
                            
                            if(Opcje::IsMeshEnabled())
                                ListView_SetExtendedListViewStyle(hWidok, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
                            else
                                ListView_SetExtendedListViewStyle(hWidok, LVS_EX_FULLROWSELECT);
                            
                            InvalidateRect(hChild, NULL, false);
                        }
                    }
                    
                    SendMessage(hClient, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_OPROGRAMIE:
                {
                    DialogBox(hThis, "ABOUT_DIALOG", hwnd, AboutProc);
                    return 0;
                }
                case IDC_SKROTY:
                {
                    SendMessage(hClient, WM_TIPDISABLE, 0, 0);
                    CreateDialog(hThis, "ACCEL_DIALOG", hwnd, AccelProc);
                    SendMessage(hClient, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_HSASIAD:
                {
                    SendMessage(hMdi, WM_MDITILE, MDITILE_HORIZONTAL, 0);
                    return 0;
                }
                case IDC_VSASIAD:
                {
                    SendMessage(hMdi, WM_MDITILE, MDITILE_VERTICAL, 0);
                    return 0;
                }
                case IDC_KASKADA:
                {
                    SendMessage(hMdi, WM_MDICASCADE, 0, 0);
                    return 0;
                }
                case IDC_EDYTOR:
                {
                    char pszFile[1024]; GetModuleFileName(hThis, pszFile, 1024);
                    PathRemoveFileSpec(pszFile);
                    
                    PathAppend(pszFile, "Manager.exe");
                    
                    ShellExecute(NULL, "open", pszFile, NULL, NULL, SW_SHOW);
                    
                    return 0;
                }
                case IDC_JEZYK1:
                {
                    SetMenu(hwnd, NULL);
                    DestroyMenu(hMain);
                    Jezyk::Zamknij();
                     
                    Jezyk::Inicjuj(NULL);
                    Jezyk::Domyslne(hThis);
                    
                    hMain = CreateMainMenu(hThis);
                    Recent::UaktualnijMenu(GetSubMenu(hMain, 0));
                        
                    SetMenu(hwnd, hMain);
                    SendMessage(hMdi, WM_MDISETMENU, 0, (LPARAM)GetSubMenu(hMain, 4));
                        
                    if(!hClient)
                    {
                        EnableMainMenu(false);
                    }    
                    else
                    {
                        SendMessage(hClient, WM_UPDATELANGUAGE, (bool)true, 0);
                        for(int i = 0; hClient = GetDlgItem(hMdi, ID_WINDOW + i); i++)
                            SendMessage(hClient, WM_UPDATELANGUAGE, (bool)false, 0);
                    }
                        
                    RKey rKey;
                    rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_WRITE);
                    rKey.ZapiszRekordC("pszLanguage", "");
                    rKey.Zamknij();
                    
                    return 0;
                }
                case IDC_POMOC:
                {
                    char pszFile[1024]; GetModuleFileName(hThis, pszFile, 1024);
                    PathRemoveFileSpec(pszFile);
                    
                    PathAppend(pszFile, "katalogus.hlp");
                    
                    ShellExecute(NULL, "open", pszFile, NULL, NULL, SW_SHOW);
                    
                    return 0;
                }
                default:
                {
                    if(LOWORD(wParam) >= IDC_JEZYK0 && LOWORD(wParam) < IDC_JEZYK1)
                    {
                        int iItem = LOWORD(wParam) - IDC_JEZYK0;
                        int i = 0;
                        
                        char pszPath[1024]; GetModuleFileName(hThis, pszPath, 1024);
    
                        PathRemoveFileSpec(pszPath);
                        PathAppend(pszPath, "translations");
    
                        SetCurrentDirectory(pszPath);
    
                        WIN32_FIND_DATA wfd; ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
                        HANDLE hFile = FindFirstFile("*.kln", &wfd);
                        
                        while(i < iItem)
                        {
                            FindNextFile(hFile, &wfd);
                            i++;
                        }
                        
                        FindClose(hFile);
                        GetFullPathName(wfd.cFileName, 1024, pszPath, NULL);  
                        
                        SetMenu(hwnd, NULL);
                        DestroyMenu(hMain);
                        Jezyk::Zamknij();
                        
                        if(!Jezyk::Inicjuj(pszPath))
                        {
                            ObslugaBledu();
                            Jezyk::Domyslne(hThis);
                        }
                        
                        hMain = CreateMainMenu(hThis);
                        Recent::UaktualnijMenu(GetSubMenu(hMain, 0));
                        
                        SetMenu(hwnd, hMain);
                        SendMessage(hMdi, WM_MDISETMENU, 0, (LPARAM)GetSubMenu(hMain, 4));
                        
                        
                        if(!hClient)
                        {
                            EnableMainMenu(false);
                        }    
                        else
                        {
                            SendMessage(hClient, WM_UPDATELANGUAGE, (bool)true, 0);
                            for(int i = 0; hClient = GetDlgItem(hMdi, ID_WINDOW + i); i++)
                                SendMessage(hClient, WM_UPDATELANGUAGE, (bool)false, 0);
                        }    
                        
                        RKey rKey;
                        rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_WRITE);
                        rKey.ZapiszRekordC("pszLanguage", pszPath);
                        rKey.Zamknij();
                        
                        return 0;
                    }
                    
                    if(LOWORD(wParam) >= ID_WINDOW)
                    {
                        DefFrameProc(hwnd, hMdi, message, wParam, lParam);
                    }
                    else
                    {
                        if(hClient) SendMessage(hClient, WM_COMMAND, wParam, lParam);
                        else
                        {
                            SetLastError(_ERROR_OVERDRIVE);
                            ObslugaBledu();
                            return 0;
                        }
                    }
                    return 0;
                }
            }
            break;
        }
    }
    return DefFrameProc(hwnd, hMdi, message, wParam, lParam);
}

#include "Jezyk.hpp"
#include "Katalog.hpp"
#include "Interfejs.hpp"
#include "Recent.hpp"
#include "MainDbProc.hpp"
#include "TipProc.hpp"
#include "ListViewProc.hpp"
#include "ItemProc.hpp"
#include "SkanujProc.hpp"
#include "SzukajProc.hpp"
#include "StatsProc.hpp"
#include "ReportProc.hpp"
#include "AboutProc.hpp"
#include "AccelProc.hpp"
#include "PsswdProc.hpp"
#include "OptionsProc.hpp"
#include "SkanujKatalog.hpp"
#include "ListView.hpp"
#include "Bledy.hpp"
#include "Opcje.hpp"
#include "DodajDoRaportu.hpp"
#include "ScanThread.hpp"
#include "LoadThread.hpp"
#include "SaveThread.hpp"
#include "SortThread.hpp"
#include "SearchThread.hpp"
#include "ReportThread.hpp"
