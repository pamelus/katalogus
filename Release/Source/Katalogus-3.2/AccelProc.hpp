BOOL CALLBACK AccelProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "ACCELS_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            SetWindowText(hwnd, Jezyk::pszDialog[31]);
            char pszAccels[] = "Ctrl + N\nCtrl + O\nCtrl + S\nCtrl + Shift + S\nCtrl + W\n\nEnter\nBackspace\nCtrl + D\nF2\nDelete\nCtrl + Up\nCtrl + Down\n\nCtrl + F\nCtrl + Shift + F\n\nF5\nF6\nF7\nF8\n\nF1";
            char pszDescribe[1024];
            wsprintf(pszDescribe, " - %s\n - %s\n - %s\n - %s\n - %s\n\n - %s\n - %s\n - %s\n - %s\n - %s\n - %s\n - %s\n\n - %s\n - %s\n\n - %s\n - %s\n - %s\n - %s\n\n - %s", Jezyk::pszDialog[79], Jezyk::pszDialog[80], Jezyk::pszDialog[81], Jezyk::pszDialog[82], Jezyk::pszDialog[83], Jezyk::pszDialog[84], Jezyk::pszDialog[85], Jezyk::pszDialog[86], Jezyk::pszDialog[87], Jezyk::pszDialog[88], Jezyk::pszDialog[89], Jezyk::pszDialog[90], Jezyk::pszDialog[91], Jezyk::pszDialog[92], Jezyk::pszDialog[93], Jezyk::pszDialog[94], Jezyk::pszDialog[95], Jezyk::pszDialog[96], Jezyk::pszDialog[97]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, pszAccels);
            SetDlgItemText(hwnd, IDBOX_STATIC2, pszDescribe);
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
