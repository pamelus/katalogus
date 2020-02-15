BOOL CALLBACK Wizard0Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static LOGFONT logfont[6];
    static COLORREF colors[6];
    switch(message)
    {
        case WM_SETINFO:
        {
            CopyMemory(logfont, (LOGFONT *)lParam, 6 * sizeof(LOGFONT));
            CopyMemory(colors, (COLORREF *)wParam, 6 * sizeof(COLORREF));
            return TRUE;
        }
        case WM_GETINFO:
        {
            CopyMemory((LOGFONT *)lParam, logfont, 6 * sizeof(LOGFONT));
            CopyMemory((COLORREF *)wParam, colors, 6 * sizeof(COLORREF));
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[33]);
            SetDlgItemText(hwnd, IDCBOX_FONT1, Jezyk::pszDialog[34]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[35]);
            SetDlgItemText(hwnd, IDCBOX_FONT2, Jezyk::pszDialog[36]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[37]);
            SetDlgItemText(hwnd, IDCBOX_FONT3, Jezyk::pszDialog[38]);
            SetDlgItemText(hwnd, IDBOX_STATIC4, Jezyk::pszDialog[39]);
            SetDlgItemText(hwnd, IDCBOX_FONT4, Jezyk::pszDialog[40]);
            SetDlgItemText(hwnd, IDBOX_STATIC5, Jezyk::pszDialog[41]);
            SetDlgItemText(hwnd, IDCBOX_FONT5, Jezyk::pszDialog[42]);
            SetDlgItemText(hwnd, IDBOX_STATIC6, Jezyk::pszDialog[43]);
            SetDlgItemText(hwnd, IDCBOX_FONT6, Jezyk::pszDialog[44]);
            SetDlgItemText(hwnd, IDBOX_STATIC7, Jezyk::pszDialog[45]);
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_FONT1:
                case IDCBOX_FONT2:
                case IDCBOX_FONT3:
                case IDCBOX_FONT4:
                case IDCBOX_FONT5:
                case IDCBOX_FONT6:
                {                    
                    int iItem = LOWORD(wParam) - IDCBOX_FONT1;
                    LOGFONT temp; CopyMemory(&temp, &logfont[iItem], sizeof(LOGFONT));
                    
                    CHOOSEFONT cf;
                    cf.lStructSize = sizeof(CHOOSEFONT);
                    cf.hwndOwner = GetParent(hwnd);
                    cf.lpLogFont = &temp;
                    cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS | CF_SCREENFONTS;
                    cf.rgbColors = colors[iItem];
                    
                    if(ChooseFont(&cf))
                    {
                        CopyMemory(&logfont[iItem], &temp, sizeof(LOGFONT));
                        colors[iItem] = cf.rgbColors;
                    }
                    
                    return TRUE;
                }    
            }
            break;
        }
    }
    return FALSE;
}

BOOL CALLBACK Wizard1Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int iDana[3] = {0, 0, 0};
    static DWORD dwUklad = 0;
    switch(message)
    {
        case WM_GETINFO:
        {
            CopyMemory((int *)lParam, iDana, 3 * sizeof(int));
            *((DWORD *)wParam) = dwUklad;
            return TRUE;
        }
        case WM_SETINFO:
        {
            CopyMemory(iDana, (int *)lParam, 3 * sizeof(int));
            dwUklad = *((DWORD *)wParam);
            
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[47]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[48]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[49]);
            SetDlgItemText(hwnd, IDBOX_STATIC6, Jezyk::pszDialog[50]);
            SetDlgItemText(hwnd, IDBOX_STATIC7, Jezyk::pszDialog[51]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX1, Jezyk::pszDialog[52]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX3, Jezyk::pszDialog[52]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX2, Jezyk::pszDialog[53]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX4, Jezyk::pszDialog[53]);
            
            HWND hList1 = GetDlgItem(hwnd, IDBOX_STATIC4);
            HWND hList2 = GetDlgItem(hwnd, IDBOX_STATIC5);
            
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[67]);
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[68]);
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[69]);
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[70]);
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[71]);
            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)Jezyk::pszDialog[72]);
            
            if(iDana[0] != 0)
            {
                char pszText[256]; SendMessage(hList1, LB_GETTEXT, iDana[0] - 1, (LPARAM)pszText);
                SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)pszText);
            }
            
            if(iDana[1] != 0)
            {
                char pszText[256]; SendMessage(hList1, LB_GETTEXT, iDana[1] - 1, (LPARAM)pszText);
                SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)pszText);
            }
            
            if(iDana[2] != 0)
            {    
                char pszText[256]; SendMessage(hList1, LB_GETTEXT, iDana[2] - 1, (LPARAM)pszText);
                SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)pszText);
            }
            
            if(LOWORD(dwUklad) != 0) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_SETCHECK, BST_CHECKED, 0);
            else SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_SETCHECK, BST_CHECKED, 0);
            
            if(HIWORD(dwUklad) != 0) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX3, BM_SETCHECK, BST_CHECKED, 0);
            else SendDlgItemMessage(hwnd, IDBOX_CHECKBOX4, BM_SETCHECK, BST_CHECKED, 0);
            
            return FALSE;
        }
        case WM_DESTROY:
        {
            WORD word1 = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
            WORD word2 = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX3, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
                
                
            dwUklad = MAKELONG(word1, word2);
            
            return TRUE;
        }
        case WM_NOTIFY:
        {
            switch(((NMHDR *)lParam)->code)
            {
                case PSN_SETACTIVE:
                {
                    if(!(SendDlgItemMessage(hwnd, IDBOX_STATIC5, LB_GETCOUNT, 0, 0) > 0))
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_BACK);
                        
                    if(SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_GETCHECK, 0, 0) == BST_UNCHECKED && SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_BACK);
                    
                    return TRUE;
                }
                case PSN_KILLACTIVE:
                {
                    SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_NEXT | PSWIZB_BACK);
                    return TRUE;
                }
            }
            break;
        }
        case WM_COMMAND:
        {
            HWND hList1 = GetDlgItem(hwnd, IDBOX_STATIC4);
            HWND hList2 = GetDlgItem(hwnd, IDBOX_STATIC5);
            switch(LOWORD(wParam))
            {
                case IDCBOX_OK:
                {
                    int iCount = SendMessage(hList2, LB_GETCOUNT, 0, 0);
                    
                    if(iCount >= 3)
                    {
                        MessageBox(hwnd, Jezyk::pszDialog[73], pszProgramName, MB_OK | MB_ICONWARNING);
                        return TRUE;
                    }
                    
                    int iItem = SendMessage(hList1, LB_GETCURSEL, 0, 0);
                    if(iItem < 0) return TRUE;
                    
                    iDana[iCount] = iItem + 1;
                    
                    char pszText[256];
                    SendMessage(hList1, LB_GETTEXT, iItem, (LPARAM)pszText);
                    SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)pszText);
                    
                    if(SendDlgItemMessage(hwnd, IDBOX_CHECKBOX1, BM_GETCHECK, 0, 0) == BST_UNCHECKED && SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_BACK);
                    else
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_NEXT | PSWIZB_BACK);
                               
                    return TRUE;
                }
                case IDCBOX_ANULUJ:
                {
                    int iItem = SendMessage(hList2, LB_GETCURSEL, 0, 0);
                    if(iItem < 0) return TRUE;
                    
                    SendMessage(hList2, LB_DELETESTRING, iItem, 0);
                    
                    iDana[iItem] = 0;
                    for(int i = iItem; i < 2; i++)
                    {
                        iDana[i] = iDana[i+1];
                        iDana[i+1] = 0;
                    }
                    
                    int iCount = SendMessage(hList2, LB_GETCOUNT, 0, 0);
                    if(iCount == 0)
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_BACK);
                    
                    return TRUE;
                }
                case IDBOX_CHECKBOX1:
                case IDBOX_CHECKBOX2:
                {
                    if(SendMessage(hList2, LB_GETCOUNT, 0, 0) > 0)
                        SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_NEXT | PSWIZB_BACK);
                    return FALSE;
                }
            }
            break;
        }    
    }
    return FALSE;
}

BOOL CALLBACK Wizard2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char pszHeaders[3][256] = {"", "", ""};
    switch(message)
    {
        case WM_SETINFO:
        {
            return TRUE;
        }
        case WM_GETINFO:
        {
            CopyMemory((char **)lParam, pszHeaders, 3 * 256);
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[55]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[56]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[57]);
            SetDlgItemText(hwnd, IDBOX_STATIC4, Jezyk::pszDialog[58]);
            return FALSE;
        }
        case WM_DESTROY:
        {
            for(int i = 0; i < 3; i++)
                GetDlgItemText(hwnd, IDBOX_STATIC5 + i, pszHeaders[i], 256);
            return TRUE;
        }
        case WM_NOTIFY:
        {
            switch(((NMHDR *)lParam)->code)
            {
                case PSN_SETACTIVE:
                {
                    int iDane[3] = {0, 0, 0};
                    DWORD dwUklad = 0;
                    
                    Wizard1Proc(NULL, WM_GETINFO, (WPARAM)&dwUklad, (LPARAM)iDane);
                    
                    for(int i = 0; i < 3; i++)
                    {
                        SendDlgItemMessage(hwnd, IDBOX_STATIC5 + i, EM_LIMITTEXT, 255, 0);
                        switch(iDane[i])
                        {
                            case 0:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), false);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), false);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, "");
                                break;
                            case 1:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[67]);
                                break;
                            case 2:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[68]);
                                break;
                            case 3:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[69]);
                                break;
                            case 4:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[70]);
                                break;
                            case 5:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[71]);
                                break;
                            case 6:
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2 + i), true);
                                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5 + i), true);
                                SetDlgItemText(hwnd, IDBOX_STATIC5 + i, Jezyk::pszDialog[72]);
                                break;
                        }
                    }
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

BOOL CALLBACK Wizard3Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool iOptions[4];
    static char pszText[2][16];
    switch(message)
    {
        case WM_SETINFO:
        {
            CopyMemory(iOptions, (bool *)wParam, 4 * sizeof(bool));
            CopyMemory(pszText, (char **)lParam, 2 * 16);
            return TRUE;
        }
        case WM_GETINFO:
        {
            CopyMemory((bool *)wParam, iOptions, 4 * sizeof(bool));
            CopyMemory((char **)lParam, pszText, 2 * 16);
            return TRUE;
        }
        case WM_INITDIALOG:
        {
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[60]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX0, Jezyk::pszDialog[61]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX1, Jezyk::pszDialog[62]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX2, Jezyk::pszDialog[63]);
            SetDlgItemText(hwnd, IDBOX_CHECKBOX3, Jezyk::pszDialog[64]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[65]);
            SetDlgItemText(hwnd, IDBOX_STATIC3, Jezyk::pszDialog[66]);
            
            SendDlgItemMessage(hwnd, IDBOX_STATIC4, EM_LIMITTEXT, 15, 0);
            SendDlgItemMessage(hwnd, IDBOX_STATIC5, EM_LIMITTEXT, 15, 0);
            
            for(int i = 0; i < 4; i++)
                if(iOptions[i]) SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0 + i, BM_SETCHECK, BST_CHECKED, 0);
            
            if(iOptions[2])
            {
                SetDlgItemText(hwnd, IDBOX_STATIC4, pszText[0]);
            }
            else
            {
                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2), false);
                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC4), false);
            }
            
            if(iOptions[3])
            {
                SetDlgItemText(hwnd, IDBOX_STATIC5, pszText[1]);
            }
            else
            {
                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC3), false);
                EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5), false);
            }
            return FALSE;
        }
        case WM_DESTROY:
        {
            for(int i = 0; i < 4; i++)
                iOptions[i] = (SendDlgItemMessage(hwnd, IDBOX_CHECKBOX0 + i, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
            
            GetDlgItemText(hwnd, IDBOX_STATIC4, pszText[0], 16);
            GetDlgItemText(hwnd, IDBOX_STATIC5, pszText[1], 16);
                
            return TRUE;
        }
        case WM_NOTIFY:
        {
            switch(((NMHDR *)lParam)->code)
            {
                case PSN_SETACTIVE:
                {
                    SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_FINISH | PSWIZB_BACK);                    
                    return TRUE;
                }
                case PSN_KILLACTIVE:
                {
                    SendMessage(GetParent(hwnd), PSM_SETWIZBUTTONS, 0, (LPARAM)PSWIZB_NEXT | PSWIZB_BACK);
                    return TRUE;
                }
            }
            break;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDBOX_CHECKBOX2:
                {
                    if(SendDlgItemMessage(hwnd, IDBOX_CHECKBOX2, BM_GETCHECK, 0, 0) == BST_CHECKED)
                    {
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2), true);
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC4), true);
                        
                        SetFocus(GetDlgItem(hwnd, IDBOX_STATIC4));
                    }
                    else
                    {
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC2), false);
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC4), false);
                        
                        SetDlgItemText(hwnd, IDBOX_STATIC4, "");
                    }
                    return FALSE;
                }
                case IDBOX_CHECKBOX3:
                {
                    if(SendDlgItemMessage(hwnd, IDBOX_CHECKBOX3, BM_GETCHECK, 0, 0) == BST_CHECKED)
                    {
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC3), true);
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5), true);
                        
                        SetFocus(GetDlgItem(hwnd, IDBOX_STATIC5));
                    }
                    else
                    {
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC3), false);
                        EnableWindow(GetDlgItem(hwnd, IDBOX_STATIC5), false);
                        
                        SetDlgItemText(hwnd, IDBOX_STATIC5, "");
                    }
                    return FALSE;
                }
            }
            break;
        }
    }
    return FALSE;
}
