BOOL CALLBACK AboutProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap = NULL;
    static BITMAP bitmap;
    static RECT rcc; // Okno
    static RECT rcd1; // Text g³ównu
    static RECT rcd2; // Text z wersj¹
    static HPEN hPen = NULL;
    static HBRUSH hBlack = NULL;
    static HBRUSH hNull = NULL;
    static HFONT hFont = NULL;
    
    static char pszText1[1024] = "";
    static char pszText2[256] = "";
    
    switch(message)
    {
        case WM_INITDIALOG:
        {
            RECT rect; GetWindowRect(hwnd, &rect);
            
            int cx = rect.right - rect.left;
            int cy = rect.bottom - rect.top;
            
            int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
            int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;
            
            MoveWindow(hwnd, x, y, cx, cy, false);
            
            GetClientRect(hwnd, &rcc);
            
            hBitmap = LoadBitmap(GetModuleHandle(NULL), "LOGO_BITMAP");
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            
            hBlack = CreateSolidBrush(0x000000);
            hNull = (HBRUSH)GetStockObject(NULL_BRUSH);
            hPen = CreatePen(PS_SOLID, 1, 0xCCCCCC);
            hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
            
            CopyMemory(&rcd1, &rcc, sizeof(RECT));
            rcd1.top += 100;
            rcd1.left += 10;
            
            rcd2.left = rcc.right - 100; rcd2.right = rcc.right - 10;
            rcd2.top = rcc.top + 5; rcd2.bottom = rcc.top + 20;
            
            char pszFile[1024]; Jezyk::GetFileName(pszFile, 1024);
            if(pszFile[0] == 0) lstrcpy(pszFile, "Domyœlny");
            wsprintf(pszText1, "Autor: Patryk Koz³owski, paŸdziernik 2004\ne-mail: pamelus86@yahoo.co.uk\nFreeware (C) Wszelkie prawa zastrze¿one dla Xamax Software Patryk Koz³owski.\n\nPlik jêzykowy: %s\nPrzet³umaczy³: %s\ne-mail: %s\n\nPodziêkowania dla: R. Flasiñskiego, G. i T. Koz³owskich, M. Scheffsa i R. Sobieszczyka.", PathFindFileName(pszFile), Jezyk::pszAutor[0], Jezyk::pszAutor[1]);
            lstrcpy(pszText2, "v. 3.2.0");
            
            return TRUE;
        }
        case WM_DESTROY:
        {
            DeleteObject(hFont);
            DeleteObject(hPen);
            DeleteObject(hNull);
            DeleteObject(hBlack);
            DeleteObject(hBitmap);
            return TRUE;
        }
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_KEYDOWN:
        case WM_COMMAND:
        {
            EndDialog(hwnd, 0);
            return TRUE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            FillRect(hdc, &rcc, hBlack);
            
            HANDLE hPrev1;
            HANDLE hPrev2;
            
            HDC hpc = CreateCompatibleDC(hdc);
            hPrev1 = SelectObject(hpc, hBitmap);
            
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hpc, 0, 0, SRCCOPY);
            
            SelectObject(hpc, hPrev1);
            DeleteDC(hpc);
            
            hPrev1 = SelectObject(hdc, hPen);
            hPrev2 = SelectObject(hdc, hNull);
            Rectangle(hdc, rcc.left, rcc.top, rcc.right, rcc.bottom);
            SelectObject(hdc, hPrev2);
            SelectObject(hdc, hPrev1);
            
            hPrev1 = SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, 0x0ACFF5);
             
            DrawText(hdc, pszText1, lstrlen(pszText1), &rcd1, DT_WORDBREAK);
            DrawText(hdc, pszText2, lstrlen(pszText2), &rcd2, DT_RIGHT);
            
            SelectObject(hdc, hPrev1);
            
            EndPaint(hwnd, &ps);
            
            return TRUE;
        }
    }
    return FALSE;
}
