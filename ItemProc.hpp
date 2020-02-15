BOOL CALLBACK ItemProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DANE * dane = NULL;
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "ITEM_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            SetWindowText(hwnd, Jezyk::pszDialog[12]);
            
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[13]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[14]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[15]);
            SetDlgItemText(hwnd, IDCBOX_OK, Jezyk::pszDialog[0]);
            SetDlgItemText(hwnd, IDCBOX_ANULUJ, Jezyk::pszDialog[1]);
            
            dane = (DANE *)lParam;
            
            SendDlgItemMessage(hwnd, IDBOX_NAZWA, EM_LIMITTEXT, 255, 0);
            SendDlgItemMessage(hwnd, IDBOX_NOSNIK, EM_LIMITTEXT, 255, 0);
            SendDlgItemMessage(hwnd, IDBOX_OPIS, EM_LIMITTEXT, 255, 0);
            
            SetDlgItemText(hwnd, IDBOX_NAZWA, dane->nazwa);
            SetDlgItemText(hwnd, IDBOX_NOSNIK, dane->nosnik);
            SetDlgItemText(hwnd, IDBOX_OPIS, dane->opis);
            
            return FALSE;
        }
        case WM_DESTROY:
        {
            dane = NULL;
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_OK:
                {
                    GetDlgItemText(hwnd, IDBOX_NAZWA, dane->nazwa, 256);
                    GetDlgItemText(hwnd, IDBOX_NOSNIK, dane->nosnik, 256);
                    GetDlgItemText(hwnd, IDBOX_OPIS, dane->opis, 256);
                    
                    if(dane->nazwa[0] == 0)
                    {
                        MessageBox(hwnd, Jezyk::pszDialog[28], pszProgramName, MB_OK | MB_ICONWARNING);
                        SetFocus(GetDlgItem(hwnd, IDBOX_NAZWA));
                        return TRUE;
                    }
                    
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
