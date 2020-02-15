BOOL CALLBACK ScanItemProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SCANTAG * scn = NULL;
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "ITEM_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            SetWindowText(hwnd, Jezyk::pszDialog[12]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[14]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[15]);
            SetDlgItemText(hwnd, IDBOX_AUTOR, Jezyk::pszDialog[24]);
            SetDlgItemText(hwnd, IDBOX_ALBUM, Jezyk::pszDialog[25]);
            SetDlgItemText(hwnd, IDBOX_AUDIO, Jezyk::pszDialog[26]);
            SetDlgItemText(hwnd, IDCBOX_OK, Jezyk::pszDialog[0]);
            SetDlgItemText(hwnd, IDCBOX_ANULUJ, Jezyk::pszDialog[1]);
            
            SetForegroundWindow(hwnd);
            
            scn = (SCANTAG *)lParam;
            
            SendDlgItemMessage(hwnd, IDBOX_NOSNIK, EM_LIMITTEXT, 255, 0);
            SendDlgItemMessage(hwnd, IDBOX_OPIS, EM_LIMITTEXT, 255, 0);
                
            SetDlgItemText(hwnd, IDBOX_NOSNIK, scn->dane.nosnik);
            SetDlgItemText(hwnd, IDBOX_OPIS, scn->dane.opis);
            
            if(scn->autor) SendDlgItemMessage(hwnd, IDBOX_AUTOR, BM_SETCHECK, BST_CHECKED, 0);
            if(scn->album) SendDlgItemMessage(hwnd, IDBOX_ALBUM, BM_SETCHECK, BST_CHECKED, 0);
            if(scn->audio) SendDlgItemMessage(hwnd, IDBOX_AUDIO, BM_SETCHECK, BST_CHECKED, 0);
            
            return FALSE;
        }
        case WM_DESTROY:
        {
            scn = NULL;
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_OK:
                {
                    GetDlgItemText(hwnd, IDBOX_NOSNIK, scn->dane.nosnik, 256);
                    GetDlgItemText(hwnd, IDBOX_OPIS, scn->dane.opis, 256);
                    
                    scn->autor = (SendDlgItemMessage(hwnd, IDBOX_AUTOR, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                    scn->album = (SendDlgItemMessage(hwnd, IDBOX_ALBUM, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                    scn->audio = (SendDlgItemMessage(hwnd, IDBOX_AUDIO, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                    
                    EndDialog(hwnd, 0);
                    
                    return TRUE;
                }
                case IDCBOX_ANULUJ:
                {
                    EndDialog(hwnd, IDCBOX_ANULUJ);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

BOOL CALLBACK MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDCBOX_YESTOALL, Jezyk::pszDialog[2]);
            SetDlgItemText(hwnd, IDCBOX_YES, Jezyk::pszDialog[3]);
            SetDlgItemText(hwnd, IDCBOX_NO, Jezyk::pszDialog[4]);
            SetDlgItemText(hwnd, IDCBOX_NOTOALL, Jezyk::pszDialog[5]);
            
            RECT rcWindow; GetWindowRect(hwnd, &rcWindow);
            
            int cx = rcWindow.right - rcWindow.left;
            int cy = rcWindow.bottom - rcWindow.top;
            
            int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
            int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;
            
            MoveWindow(hwnd, x, y, cx, cy, false);
            SetWindowText(hwnd, pszProgramName);
            
            HMENU hSys = GetSystemMenu(hwnd, false);
            EnableMenuItem(hSys, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
            
            HICON hIcon = LoadIcon(NULL, IDI_QUESTION);
            SendDlgItemMessage(hwnd, IDBOX_STATIC1, STM_SETIMAGE, IMAGE_ICON, (LPARAM) hIcon);
            DestroyIcon(hIcon);
            
            SetDlgItemText(hwnd, IDBOX_STATIC2, (char *)lParam);
            
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_YESTOALL:
                case IDCBOX_YES:
                case IDCBOX_NO:
                case IDCBOX_NOTOALL:
                {
                    EndDialog(hwnd, LOWORD(wParam));
                    return TRUE;
                }
            }
            break;
        }    
    }
    return FALSE;
}

BOOL CALLBACK ScanProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        static SCANINITTAG * sInit = NULL;
        case WM_INITDIALOG:
        {   
            char pszFile[1024];
            GetModuleFileName(GetModuleHandle(NULL), pszFile, 1024);
            PathRemoveFileSpec(pszFile);
            PathAppend(pszFile, "extensions.txt");
            
            sInit = (SCANINITTAG *)lParam;
            if(!sInit) EndDialog(hwnd, 0);
            sInit->scn.koniec = false;
            sInit->scn.hDialog = hwnd;
            sInit->scn.pExt = fopen(pszFile, "rt");
            
            SetWindowText(hwnd, Jezyk::pszDialog[27]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[27]);
            SetDlgItemText(hwnd, IDCBOX_ANULUJ, Jezyk::pszDialog[1]);
                
            RECT rect; GetWindowRect(hwnd, &rect);
            
            int cx = rect.right - rect.left;
            int cy = rect.bottom - rect.top;
            
            int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
            int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;
            
            MoveWindow(hwnd, x, y, cx, cy, true);
            
            DWORD value = 0;
            CreateThread(NULL, 0, ScanThread, (LPVOID)sInit, 0, &value);
            
            return TRUE;
        }
        case WM_SETPATH:
        {
            char * pszPath = (char *)lParam;
            SetDlgItemText(hwnd, IDBOX_STATIC2, pszPath);
            
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_ANULUJ:
                {
                    sInit->scn.koniec = true;
                    return TRUE;
                }
                case IDC_KONIEC:
                {
                    fclose(sInit->scn.pExt);
                    EndDialog(hwnd, 0);
                    return TRUE;
                }
            }
            break;
        }    
    }
    return FALSE;
}
