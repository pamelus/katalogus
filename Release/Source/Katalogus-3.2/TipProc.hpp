LRESULT CALLBACK TipProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char pszOpis[256];
    static RECT rcText;
    static RECT rcWindow;
    static HFONT hFont = NULL;
    static HPEN hPen;
    static HBRUSH hBrush;
    switch(message)
    {
        case WM_CREATE:
        {
            LOGFONT lFont; Opcje::GetFont2(&lFont);
            hFont = CreateFontIndirect(&lFont);
            hPen = CreatePen(PS_SOLID, 1, 0x000000);
            hBrush = CreateSolidBrush(Opcje::GetBkColor2());
            return 0;
        }
        case WM_DESTROY:
        {
            DeleteObject(hBrush);
            DeleteObject(hPen);
            DeleteObject(hFont);
            hFont = NULL;
            return 0;
        }
        case WM_SETTEXT:
        {
            lstrcpyn(pszOpis, (char *)lParam, 256);
            
            int cx = 0;
            int cy = 0;
            int iBegin = 0;
            int iSpace = 0;
            
            SIZE size;
            HDC hdc = GetDC(hwnd);
            HANDLE hPrev = SelectObject(hdc, hFont);
            
            for(int i = 0; i < lstrlen(pszOpis) + 1; i++)
            {
                GetTextExtentPoint32(hdc, &pszOpis[iBegin], i - iBegin, &size);
                
                if(size.cx > 200)
                {                  
                    GetTextExtentPoint32(hdc, &pszOpis[iBegin], iSpace - iBegin, &size);
                    if(cx < size.cx) cx = size.cx;
                    cy += size.cy;
                    
                    iBegin = iSpace + 1;
                }
                
                if(pszOpis[i] == ' ') iSpace = i;
            }
            
            if(cx == 0) cx = size.cx;
            cy += size.cy;
            
            SelectObject(hdc, hPrev);
            ReleaseDC(hwnd, hdc);
            
            POINT p; GetCursorPos(&p);
            MoveWindow(hwnd, p.x + 10, p.y + 10, cx + 10, cy + 10, false);
            
            GetClientRect(hwnd, &rcWindow);
            
            rcText.left = rcWindow.left + 5;
            rcText.right = rcWindow.right - 5;
            rcText.top = rcWindow.top + 5;
            rcText.bottom = rcWindow.bottom - 5;
            
            return 0;
        }
        case WM_PAINT:
        {
            HANDLE hPrev1 = NULL;
            HANDLE hPrev2 = NULL;
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            hPrev1 = SelectObject(hdc, hPen);
            hPrev2 = SelectObject(hdc, hBrush);
            
            Rectangle(hdc, rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom);
            
            SelectObject(hdc, hPrev1);
            SelectObject(hdc, hPrev2);
            
            hPrev1 = SelectObject(hdc, hFont);
            
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, Opcje::GetTextColor2());
            DrawText(hdc, pszOpis, lstrlen(pszOpis), &rcText, DT_WORDBREAK);
            
            SelectObject(hdc, hPrev1);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            HWND hData = GetParent(hwnd);
            SendMessage(hData, WM_TIPDESTROY, 0, 0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
