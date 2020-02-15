BOOL CALLBACK PsswdProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char pszPass[256];
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "OPTIONS_SAFETY_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            SetWindowText(hwnd, Jezyk::pszDialog[98]);
            
            SetDlgItemText(hwnd, IDCBOX_OK, Jezyk::pszDialog[0]);
            SetDlgItemText(hwnd, IDCBOX_ANULUJ, Jezyk::pszDialog[1]);
            
            char pszParam[2][1024];
            CopyMemory(pszParam, (char **)lParam, 2048);
            lstrcpyn(pszPass, pszParam[0], 256);
            
            RECT rcText; GetWindowRect(GetDlgItem(hwnd, IDBOX_STATIC1), &rcText);
            HDC hdc = GetDC(GetDlgItem(hwnd, IDBOX_STATIC1));
            PathCompactPath(hdc, pszParam[1], rcText.right - rcText.left - 10);
            ReleaseDC(GetDlgItem(hwnd, IDBOX_STATIC1), hdc);
            
            char pszTitle[2048];
            wsprintf(pszTitle, "%s\n%s", Jezyk::pszDialog[99], pszParam[1]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, pszTitle);
            
            SetFocus(GetDlgItem(hwnd, IDBOX_PSSWD0));
            
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_OK:
                {
                    char pszText[256]; GetDlgItemText(hwnd, IDBOX_PSSWD0, pszText, 256);
                    
                    if(lstrcmp(pszText, pszPass) == 0)
                    {
                        EndDialog(hwnd, IDCBOX_OK);
                    }
                    else
                    {
                        MessageBox(hwnd, Jezyk::pszDialog[100], pszProgramName, MB_OK | MB_ICONWARNING);
                        
                        SetDlgItemText(hwnd, IDBOX_PSSWD0, "");
                        SetFocus(GetDlgItem(hwnd, IDBOX_PSSWD0));
                    }
                    
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
