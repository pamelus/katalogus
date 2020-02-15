BOOL CALLBACK FindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static FINDTAG * fnd = NULL;
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "SEARCH_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            SetWindowText(hwnd, Jezyk::pszDialog[16]);
            SetDlgItemText(hwnd, IDBOX_STATIC1, Jezyk::pszDialog[17]);
            SetDlgItemText(hwnd, IDCBOX_OK, Jezyk::pszDialog[18]);
            SetDlgItemText(hwnd, IDBOX_STATIC2, Jezyk::pszDialog[19]);
            SetDlgItemText(hwnd, IDBOX_AUTOR, Jezyk::pszDialog[20]);
            SetDlgItemText(hwnd, IDBOX_ALBUM, Jezyk::pszDialog[21]);
            SetDlgItemText(hwnd, IDBOX_AUDIO, Jezyk::pszDialog[22]);
            
            fnd = (FINDTAG *)lParam;
            
            SendDlgItemMessage(hwnd, IDBOX_NAZWA, EM_LIMITTEXT, 255, 0);
            SetDlgItemText(hwnd, IDBOX_NAZWA, fnd->pszFraza);
            
            if(fnd->autor) SendDlgItemMessage(hwnd, IDBOX_AUTOR, BM_SETCHECK, BST_CHECKED, 0);
            if(fnd->album) SendDlgItemMessage(hwnd, IDBOX_ALBUM, BM_SETCHECK, BST_CHECKED, 0);
            if(fnd->audio) SendDlgItemMessage(hwnd, IDBOX_AUDIO, BM_SETCHECK, BST_CHECKED, 0);
            
            return FALSE;            
        }
        case WM_DESTROY:
        {
            fnd = NULL;
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCBOX_OK:
                {
                    GetDlgItemText(hwnd, IDBOX_NAZWA, fnd->pszFraza, 256);
                    if(fnd->pszFraza[0] == 0)
                    {
                        MessageBox(hwnd, Jezyk::pszDialog[29], pszProgramName, MB_OK | MB_ICONWARNING);
                        SetFocus(GetDlgItem(hwnd, IDBOX_NAZWA));
                        
                        return TRUE;
                    }
                    
                    fnd->autor = (SendDlgItemMessage(hwnd, IDBOX_AUTOR, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                    fnd->album = (SendDlgItemMessage(hwnd, IDBOX_ALBUM, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                    fnd->audio = (SendDlgItemMessage(hwnd, IDBOX_AUDIO, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;
                                        
                    EndDialog(hwnd, 0);
                    
                    return TRUE;
                }
                case IDCBOX_ANULUJ:
                {
                    EndDialog(hwnd, IDCBOX_ANULUJ);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL CALLBACK ResultProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), "RESULTS_ICON");
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);
            
            HINSTANCE hThis = GetModuleHandle(NULL);
            FINDTAG * fnd = (FINDTAG *)lParam;
            
            char pszCaption[1024];
            wsprintf(pszCaption, Jezyk::pszDialog[23], fnd->pszFraza, fnd->iRes);
            SetWindowText(hwnd, pszCaption);
                        
            HWND hWidok = GetDlgItem(hwnd, ID_WIDOK);
            ListView_SetExtendedListViewStyle(hWidok, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
            
            LV_COLUMN lvc;
            lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
            lvc.fmt = 0;
            lvc.cx = 150;
            lvc.cchTextMax = 256;
    
            lvc.pszText = Jezyk::pszColumn[0];
            lvc.iSubItem = 0;
            ListView_InsertColumn(hWidok, 0, &lvc);
            
            lvc.pszText = Jezyk::pszColumn[1];
            lvc.iSubItem = 1;
            lvc.cx = 100;
            ListView_InsertColumn(hWidok, 1, &lvc);
    
            lvc.pszText = Jezyk::pszColumn[3];
            lvc.iSubItem = 2;
            lvc.cx = 150;
            ListView_InsertColumn(hWidok, 2, &lvc);
            
            lvc.pszText = Jezyk::pszColumn[4];
            lvc.iSubItem = 3;
            lvc.cx = 100;
            ListView_InsertColumn(hWidok, 3, &lvc);
    
            hIcon = NULL;
            HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR16 | ILC_MASK, 1, 1);
    
            hIcon = LoadIcon(hThis, "AUTOR_ICON");
            ImageList_AddIcon(hImageList, hIcon);
            DestroyIcon(hIcon);
            
            hIcon = LoadIcon(hThis, "ALBUM_ICON");
            ImageList_AddIcon(hImageList, hIcon);
            DestroyIcon(hIcon);
            
            hIcon = LoadIcon(hThis, "AUDIO_ICON");
            ImageList_AddIcon(hImageList, hIcon);
            DestroyIcon(hIcon);
    
            ListView_SetImageList(hWidok, hImageList, LVSIL_SMALL);
            
            LV_ITEM lvi;
            lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
            lvi.iSubItem = 0;
            lvi.state = 0;
            lvi.stateMask = 0;
            
            for(int i = 0; i < fnd->iRes; i++)
            {
                lvi.iItem = i;
                lvi.iImage = fnd->rslt[i].iImage;
                ListView_InsertItem(hWidok, &lvi);
                
                ListView_SetItemText(hWidok, i, 0, fnd->rslt[i].pszNazwa);
                ListView_SetItemText(hWidok, i, 1, fnd->rslt[i].pszNosnik);
                ListView_SetItemText(hWidok, i, 2, fnd->rslt[i].pszSciezka);
                
                char pszIndex[16];
                wsprintf(pszIndex, "%i", i+1);
                
                ListView_SetItemText(hWidok, i, 3, pszIndex);
            }
            
            return FALSE;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return TRUE;
        }
        case WM_DESTROY:
        {
            return TRUE;
        }
        case WM_SETFOCUS:
        {
            SetFocus(GetDlgItem(hwnd, ID_WIDOK));
            return TRUE;
        }
        case WM_SIZING:
        {
            RECT * rect = (RECT *)lParam;
            if((rect->bottom - rect->top) < 200) rect->bottom = rect->top + 200;
            if((rect->right - rect->left) < 250) rect->right = rect->left + 250;
            return TRUE;
        }
        case WM_SIZE:
        {
            int cx = LOWORD(lParam);
            int cy = HIWORD(lParam);
            
            MoveWindow(GetDlgItem(hwnd, ID_WIDOK), 0, 0, cx, cy, true);
            
            return TRUE;
        }
        case WM_NOTIFY:
        {
            NMHDR * nmhdr = (LPNMHDR)lParam;            
            switch(nmhdr->code)
            {
                case LVN_COLUMNCLICK:
                {
                    NMLISTVIEW * nmlv = (LPNMLISTVIEW)lParam;
                    if(nmlv->iSubItem > 2) return TRUE;
                    
                    HWND hWidok = GetDlgItem(hwnd, ID_WIDOK);
                    LV_ITEM lvi;
                    lvi.mask = LVIF_PARAM;
                    lvi.iSubItem = 0;
                    
                    for(int i = 0; i < ListView_GetItemCount(hWidok); i++)
                    {
                        lvi.iItem = i;
                        lvi.lParam = (LPARAM)new char[256];
                        ListView_GetItemText(hWidok, i, nmlv->iSubItem, (char *)lvi.lParam, 256);
                        ListView_SetItem(hWidok, &lvi);
                    }
                    
                    ListView_SortItems(hWidok, Compare, 0);
                    
                    for(int i = 0; i < ListView_GetItemCount(hWidok); i++)
                    {
                        lvi.iItem = i;
                        lvi.lParam = 0;
                        ListView_GetItem(hWidok, &lvi);
                        
                        delete [] (char *)lvi.lParam;
                        lvi.lParam = 0;
                        ListView_SetItem(hWidok, &lvi);
                        
                        char pszIndex[32];
                        wsprintf(pszIndex, "%i", i + 1);
                        
                        ListView_SetItemText(hWidok, i, 3, pszIndex);                        
                    }    
                    
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}
