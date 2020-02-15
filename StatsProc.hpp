BOOL CALLBACK StatsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {           
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "STATS_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
             
            STATSTAG * sts = (STATSTAG *)lParam;
            
            char pszSrednia1[16]; FloatToString(sts->iAlbum, sts->iAutor, pszSrednia1, 16);
            char pszSrednia2[16]; FloatToString(sts->iAudio, sts->iAlbum, pszSrednia2, 16);
            
            char pszMessage[2048];
            wsprintf(pszMessage, Jezyk::pszDialog[30], sts->pszDatabase);
            
            SetWindowText(hwnd, pszMessage);
            
            wsprintf(pszMessage, "%s %i\n%s %i\n%s %i\n\n%s %s\n%s %s", Jezyk::pszDialog[74], sts->iAutor, Jezyk::pszDialog[75], sts->iAlbum, Jezyk::pszDialog[76], sts->iAudio, Jezyk::pszDialog[77], pszSrednia1, Jezyk::pszDialog[78], pszSrednia2);
            SetDlgItemText(hwnd, IDBOX_STATIC2, pszMessage);
            SetDlgItemText(hwnd, IDCBOX_OK, Jezyk::pszDialog[0]);
            
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_ANULUJ:
                case IDCBOX_OK:
                    DestroyWindow(hwnd);
                    return TRUE;
            }    
            break;
        }    
    }    
    return FALSE;
}

