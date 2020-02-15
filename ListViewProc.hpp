LRESULT CALLBACK ListViewProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static WNDPROC wViewProc = NULL;
    switch(message)
    {
        case WM_INITDIALOG:
        {
            wViewProc = (WNDPROC)lParam;
            return 0;
        }
        case WM_DESTROY:
        {
            DeleteObject((HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0));
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            SendMessage(GetParent(hwnd), WM_TIPDESTROY, 0, 0);
            if(message == WM_RBUTTONDOWN)
            {
                PostMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(IDC_MENU, 0), 0);
            }
            break;
        }
        case WM_LBUTTONDBLCLK:
        {
            SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(IDC_OTWORZ, 0), 0);
            return 0;
        }
        case WM_DROPFILES:
        {
            SendMessage(GetParent(hwnd), message, wParam, lParam);
            return 0;
        }
    }
    if(wViewProc) return CallWindowProc(wViewProc, hwnd, message, wParam, lParam);
    else return DefWindowProc(hwnd, message, wParam, lParam);
}
