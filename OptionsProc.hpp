BOOL CALLBACK Options1Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool isSerialAutor = false;
    static bool isSerialAlbum = false;
    static bool isSerialAudio = true;
    static bool isFolderAutor = false;
    static bool isFolderAlbum = false;
    static bool isFolderAudio = false;
    static bool isWarningEnabled = true;
    static bool isCommentEnabled = true;
    static bool isMeshEnabled = true;
    static bool isSizeEnabled = true;
    
    switch(message)
    {
        case WM_SETINFO:
        {
            isSerialAutor = Opcje::IsSerialAutor();
            isSerialAlbum = Opcje::IsSerialAlbum();
            isSerialAudio = Opcje::IsSerialAudio();
            isFolderAutor = Opcje::IsFolderAutor();
            isFolderAlbum = Opcje::IsFolderAlbum();
            isFolderAudio = Opcje::IsFolderAudio();
            isWarningEnabled = Opcje::IsWarningEnabled();
            isCommentEnabled = Opcje::IsCommentEnabled();
            isMeshEnabled = Opcje::IsMeshEnabled();
            isSizeEnabled = Opcje::IsSizeEnabled();
            return TRUE;
        }
        case WM_GETINFO:
        {
            Opcje::SetSerialAutor(isSerialAutor);
            Opcje::SetSerialAlbum(isSerialAlbum);
            Opcje::SetSerialAudio(isSerialAudio);
            Opcje::SetFolderAutor(isFolderAutor);
            Opcje::SetFolderAlbum(isFolderAlbum);
            Opcje::SetFolderAudio(isFolderAudio);
            Opcje::SetWarningEnabled(isWarningEnabled);
            Opcje::SetCommentEnabled(isCommentEnabled);
            Opcje::SetMeshEnabled(isMeshEnabled);
            Opcje::SetSizeEnabled(isSizeEnabled);
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_CHECKBOX0, Jezyk::pszDialog[105]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX1, Jezyk::pszDialog[106]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX2, Jezyk::pszDialog[107]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX3, Jezyk::pszDialog[108]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX4, Jezyk::pszDialog[109]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX5, Jezyk::pszDialog[110]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX6, Jezyk::pszDialog[111]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX7, Jezyk::pszDialog[112]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX8, Jezyk::pszDialog[113]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX9, Jezyk::pszDialog[114]);
            
            if(isSerialAutor) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0, BM_SETCHECK, BST_CHECKED, 0);
            if(isSerialAlbum) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_SETCHECK, BST_CHECKED, 0);
            if(isSerialAudio) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_SETCHECK, BST_CHECKED, 0);
            if(isFolderAutor) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX3, BM_SETCHECK, BST_CHECKED, 0);
            if(isFolderAlbum) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX4, BM_SETCHECK, BST_CHECKED, 0);
            if(isFolderAudio) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX5, BM_SETCHECK, BST_CHECKED, 0);
            if(isWarningEnabled) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX6, BM_SETCHECK, BST_CHECKED, 0);
            if(isCommentEnabled) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX7, BM_SETCHECK, BST_CHECKED, 0);
            if(isMeshEnabled) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX8, BM_SETCHECK, BST_CHECKED, 0);
            if(isSizeEnabled) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX9, BM_SETCHECK, BST_CHECKED, 0);
            return FALSE;
        }
        case WM_DESTROY:
        {
            isSerialAutor = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isSerialAlbum = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isSerialAudio = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isFolderAutor = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX3, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isFolderAlbum = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX4, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isFolderAudio = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX5, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isWarningEnabled = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX6, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isCommentEnabled = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX7, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isMeshEnabled = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX8, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            isSizeEnabled = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX9, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                
            return TRUE;
        }
    }
    
    return FALSE;
}

BOOL CALLBACK Options2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static COLORREF bColor1 = 0xFFFFFF;
    static COLORREF bColor2 = 0x0ACFF5;
    static COLORREF tColor1 = 0x000000;
    static COLORREF tColor2 = 0x000000;
    static LOGFONT lFont1 = { 8, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif" };
    static LOGFONT lFont2 = { 8, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif" };
    switch(message)
    {
        case WM_SETINFO:
        {
            bColor1 = Opcje::GetBkColor1();
            bColor2 = Opcje::GetBkColor2();
            tColor1 = Opcje::GetTextColor1();
            tColor2 = Opcje::GetTextColor2();
            Opcje::GetFont1(&lFont1);
            Opcje::GetFont2(&lFont2);
            return TRUE;
        }
        case WM_GETINFO:
        {
            Opcje::SetBkColor1(bColor1);
            Opcje::SetBkColor2(bColor2);
            Opcje::SetTextColor1(tColor1);
            Opcje::SetTextColor2(tColor2);
            Opcje::SetFont1(&lFont1);
            Opcje::SetFont2(&lFont2);
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_STATIC0, Jezyk::pszDialog[115]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[116]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[117]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[118]);
            SetDlgItemText(hwnd, IDBOX_STATIC4, Jezyk::pszDialog[119]);
            SetDlgItemText(hwnd, IDBOX_STATIC5, Jezyk::pszDialog[117]);
            SetDlgItemText(hwnd, IDBOX_STATIC6, Jezyk::pszDialog[118]);
            SetDlgItemText(hwnd, IDBOX_STATIC7, Jezyk::pszDialog[119]);
            return FALSE;
        }
        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT * di = (DRAWITEMSTRUCT *)lParam;
            switch((UINT)wParam)
            {
                case IDCBOX_COLOR1:
                case IDCBOX_COLOR2:
                case IDCBOX_COLOR3:
                case IDCBOX_COLOR4:
                {
                    COLORREF * color = NULL;
                    switch((UINT)wParam)
                    {
                        case IDCBOX_COLOR1:
                            color = &bColor1;
                            break;
                        case IDCBOX_COLOR2:
                            color = &tColor1;
                            break;
                        case IDCBOX_COLOR3:
                            color = &bColor2;
                            break;
                        case IDCBOX_COLOR4:
                            color = &tColor2;
                            break;
                    }
                    
                    HBRUSH hBrush = CreateSolidBrush(*color);
                    HPEN hPen = CreatePen(PS_SOLID, 1, 0x000000);
                    
                    HANDLE hPrev1 = SelectObject(di->hDC, hBrush);
                    HANDLE hPrev2 = SelectObject(di->hDC, hPen);
                    
                    Rectangle(di->hDC, di->rcItem.left, di->rcItem.top, di->rcItem.right, di->rcItem.bottom);
                    
                    SelectObject(di->hDC, hPrev2);
                    SelectObject(di->hDC, hPrev1);
                    
                    DeleteObject(hPen);
                    DeleteObject(hBrush);
                    return TRUE;
                }
                case IDCBOX_FONT1:
                case IDCBOX_FONT2:
                {
                    COLORREF * bColor = NULL;
                    COLORREF * tColor = NULL;
                    LOGFONT * lFont = NULL;
                    switch((UINT)wParam)
                    {
                        case IDCBOX_FONT1:
                            bColor = &bColor1;
                            tColor = &tColor1;
                            lFont = &lFont1;
                            break;
                        case IDCBOX_FONT2:
                            bColor = &bColor2;
                            tColor = &tColor2;
                            lFont = &lFont2;
                            break;
                    }
                    
                    HFONT hFont = CreateFontIndirect(lFont);
                    HBRUSH hBrush = CreateSolidBrush(*bColor);
                    HPEN hPen = CreatePen(PS_SOLID, 1, 0x000000);
                    
                    HANDLE hPrev1 = SelectObject(di->hDC, hFont);
                    HANDLE hPrev2 = SelectObject(di->hDC, hBrush);
                    HANDLE hPrev3 = SelectObject(di->hDC, hPen);
                    
                    Rectangle(di->hDC, di->rcItem.left, di->rcItem.top, di->rcItem.right, di->rcItem.bottom);
                    
                    
                    
                    SetBkMode(di->hDC, TRANSPARENT);
                    SetTextColor(di->hDC, *tColor);
                    
                    RECT rcText; CopyMemory(&rcText, &di->rcItem, sizeof(RECT));
                    SIZE size; GetTextExtentPoint32(di->hDC, lFont->lfFaceName, lstrlen(lFont->lfFaceName), &size);
                    
                    rcText.top += ((rcText.bottom - rcText.top) / 2 - size.cy / 2);
                    rcText.bottom = rcText.top + size.cy;
                    
                    DrawText(di->hDC, lFont->lfFaceName, lstrlen(lFont->lfFaceName), &rcText, DT_CENTER | DT_VCENTER);
                    
                    SelectObject(di->hDC, hPrev3);
                    SelectObject(di->hDC, hPrev2);
                    SelectObject(di->hDC, hPrev1);
                    
                    DeleteObject(hPen);
                    DeleteObject(hBrush);
                    DeleteObject(hFont);
                    return TRUE;
                }
            }
            break;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_FONT1:
                case IDCBOX_FONT2:
                {
                    LOGFONT * lFont = NULL;
                    switch(LOWORD(wParam))
                    {
                        case IDCBOX_FONT1:
                            lFont = &lFont1;
                            break;
                        case IDCBOX_FONT2:
                            lFont = &lFont2;
                            break;
                    }
                    
                    LOGFONT tFont; CopyMemory(&tFont, lFont, sizeof(LOGFONT));
                    
                    CHOOSEFONT cf;
                    cf.lStructSize = sizeof(CHOOSEFONT);
                    cf.hwndOwner = hwnd;
                    cf.lpLogFont = &tFont;
                    cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS | CF_SCREENFONTS;
                    
                    if(ChooseFont(&cf))
                    {
                        CopyMemory(lFont, &tFont, sizeof(LOGFONT));
                        InvalidateRect(GetDlgItem(hwnd, LOWORD(wParam)), NULL, false);
                    }    
                    
                    return TRUE;
                }
                case IDCBOX_COLOR1:
                case IDCBOX_COLOR2:
                case IDCBOX_COLOR3:
                case IDCBOX_COLOR4:
                {
                    static COLORREF custom[16];
                    
                    RKey rKey;
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_READ);
                    rKey.WczytajRekordD("CustomColors", custom, 16 * sizeof(COLORREF));
                    rKey.Zamknij();
                    
                    COLORREF * color = NULL;
                    
                    switch(LOWORD(wParam))
                    {
                        case IDCBOX_COLOR1:
                            color = &bColor1;
                            break;
                        case IDCBOX_COLOR2:
                            color = &tColor1;
                            break;
                        case IDCBOX_COLOR3:
                            color = &bColor2;
                            break;
                        case IDCBOX_COLOR4:
                            color = &tColor2;
                            break;
                    }
                    
                    CHOOSECOLOR cc;
                    cc.lStructSize = sizeof(CHOOSECOLOR);
                    cc.hwndOwner = hwnd;
                    cc.rgbResult = *color;
                    cc.lpCustColors = custom;
                    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                    
                    if(ChooseColor(&cc))
                    {
                        *color = cc.rgbResult;
                        
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_COLOR1), NULL, false);
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_COLOR2), NULL, false);
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_COLOR3), NULL, false);
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_COLOR4), NULL, false);
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_FONT1), NULL, false);
                        InvalidateRect(GetDlgItem(hwnd, IDCBOX_FONT2), NULL, false);
                    }
                    
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_WRITE);
                    rKey.ZapiszRekordD("CustomColors", custom, 16 * sizeof(COLORREF));
                    rKey.Zamknij();
                    
                    return TRUE;
                }    
            }    
            break;
        }
    }
    return FALSE;    
}

BOOL CALLBACK Options3Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Katalog * katalog = NULL;
    static char pszPass[256];
    static bool isChanged = false;
    switch(message)
    {
        case WM_GETINFO:
        {
            if(katalog)
            {
                if(pszPass[0] == 0)
                    katalog->UstawHaslo(NULL);
                else
                    katalog->UstawHaslo(pszPass);
                    
                if(isChanged)
                {
                    katalog->UstawModyfikacja(true);
                    
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                }
            }
            
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_CHECKBOX0, Jezyk::pszDialog[120]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[121]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[122]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[123]);
            SetDlgItemText(hwnd, IDBOX_STATIC5, Jezyk::pszDialog[124]);
            isChanged = false;
            
            SendDlgItemMessage(hwnd, IDBOX_PSSWD0, EM_LIMITTEXT, 255, 0);
            SendDlgItemMessage(hwnd, IDBOX_PSSWD1, EM_LIMITTEXT, 255, 0);
            
            HWND hClient = (HWND)SendDlgItemMessage(hOkna, ID_MDI, WM_MDIGETACTIVE, 0, 0);
            katalog = (Katalog *)GetWindowLong(hClient, GWL_USERDATA);
            if(!katalog)
            {
                EnableSafetyBox(hwnd, false);
                EnableWindow(GetDlgItem(hwnd, IDBOX_CHECKBOX0), false);
                return FALSE;
            }
            katalog->PobierzHaslo(pszPass, 256);
            
            if(pszPass[0] == 0)
            {
                EnableSafetyBox(hwnd, false);
            }
            else
            {
                EnableSafetyBox(hwnd, true);
                SetDlgItemText(hwnd, IDBOX_PSSWD0, pszPass);
                SetDlgItemText(hwnd, IDBOX_PSSWD1, pszPass);
            }
            
            return FALSE;
        }
        case WM_DESTROY:
        {
            char pszTemp[256]; lstrcpy(pszTemp, pszPass);
            GetDlgItemText(hwnd, IDBOX_PSSWD0, pszPass, 256);
            
            if(lstrcmp(pszPass, pszTemp) != 0)
                isChanged = true;
                            
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch(HIWORD(wParam))
            {
                case EN_UPDATE:
                {
                    char pszPass1[256];
                    char pszPass2[256];
                    
                    HWND hOk = GetDlgItem(GetParent(hwnd), IDOK);
                    
                    GetDlgItemText(hwnd, IDBOX_PSSWD0, pszPass1, 256);
                    GetDlgItemText(hwnd, IDBOX_PSSWD1, pszPass2, 256);
                    
                    if(pszPass1[0] == 0 && pszPass2[0] == 0)
                    {
                        EnableWindow(hOk, true);
                        SetDlgItemText(hwnd, IDBOX_STATIC4, "");
                        return 0;
                    }
                    
                    if(lstrcmp(pszPass1, pszPass2) == 0)
                    {
                        SetDlgItemText(hwnd, IDBOX_STATIC4, Jezyk::pszDialog[125]);
                        EnableWindow(hOk, true);
                    }
                    else
                    {
                        SetDlgItemText(hwnd, IDBOX_STATIC4, Jezyk::pszDialog[126]);
                        EnableWindow(hOk, false);
                    }
                    
                    return TRUE;
                }
                default:
                {
                    switch(LOWORD(wParam))
                    {
                        case IDBOX_CHECKBOX0:
                        {
                            int iState = SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0, BM_GETCHECK, 0, 0);
                            
                            if(iState == BST_CHECKED)
                            {
                                EnableSafetyBox(hwnd, false);
                            }
                            else
                            {
                                EnableSafetyBox(hwnd, true);
                            }
                            
                            return TRUE;
                        }
                    }
                }
            }
            break;
        }
    }
    return FALSE;
}

bool EnableSafetyBox(HWND hwnd, bool iEnable)
{
    EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC1), iEnable);
    EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2), iEnable);
    EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC3), iEnable);
    EnableWindow(GetDlgItem(hwnd, IDBOX_PSSWD0), iEnable);
    EnableWindow(GetDlgItem(hwnd, IDBOX_PSSWD1), iEnable);
    
    SetDlgItemText(hwnd, IDBOX_STATIC4, "");
    SetDlgItemText(hwnd, IDBOX_PSSWD0, "");
    SetDlgItemText(hwnd, IDBOX_PSSWD1, "");
    
    int iState = (iEnable) ? BST_CHECKED : BST_UNCHECKED;
                                
    SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0, BM_SETCHECK, iState, 0);
    
    return true;
}
