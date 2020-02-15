HMENU CreateMainMenu(HINSTANCE hThis)
{
    HMENU hMenu = LoadMenu(hThis, "MainMenu");
    if(!hMenu)
    {
        SetLastError(_ERROR_NOTCREATED);
        return NULL;
    }
    
    int i = 0, j = 0, k = 0;
    char pszText[256];
    
    MENUITEMINFO mii; ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;
    mii.dwTypeData = pszText;
    mii.cch = 256;
    
    while(GetMenuItemInfo(hMenu, i, TRUE, &mii))
    {
        mii.cch = 256;
        mii.dwTypeData = pszText;
        
        if(mii.fType == MFT_STRING)
        {
            lstrcpy(pszText, Jezyk::pszMenu[k]);
            SetMenuItemInfo(hMenu, i, TRUE, &mii);
            k++;
        }
            
        HMENU hSub = GetSubMenu(hMenu, i); j = 0;
        while(GetMenuItemInfo(hSub, j, TRUE, &mii))
        {
            mii.cch = 256;
            mii.dwTypeData = pszText;
            
            if(mii.fType == MFT_STRING)
            {
                lstrcpy(pszText, Jezyk::pszMenu[k]);
                SetMenuItemInfo(hSub, j, TRUE, &mii);
                k++;
            }
            
            j++;
        }
        
        i++;
    }
    
    HMENU hLan = GetSubMenu(GetSubMenu(hMenu, 3), 1);
    RemoveMenu(hLan, IDC_JEZYK0, MF_BYCOMMAND);
    
    char pszPath[1024]; GetModuleFileName(hThis, pszPath, 1024);
    
    PathRemoveFileSpec(pszPath);
    PathAppend(pszPath, "translations");
    
    SetCurrentDirectory(pszPath);
    
    WIN32_FIND_DATA wfd; ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
    HANDLE hFile = FindFirstFile("*.kln", &wfd);
    
    if(hFile != INVALID_HANDLE_VALUE)
    {
         int i = 0;
         do
         {             
             char pszPath[1024]; GetFullPathName(wfd.cFileName, 1024, pszPath, NULL);
             
             UINT uStyle = MF_STRING | (Jezyk::IsValidFileName(pszPath)) ? MF_CHECKED | MF_DISABLED : MF_UNCHECKED | MF_ENABLED;
             
             PathMakePretty(pszPath);
             lstrcpy(pszPath, PathFindFileName(pszPath));
             PathRemoveExtension(pszPath);
             
             AppendMenu(hLan, uStyle, IDC_JEZYK0 + i, pszPath);
             i++;
         }
         while((FindNextFile(hFile, &wfd)) && (i < IDC_JEZYK1 - IDC_JEZYK0 - 1));
         AppendMenu(hLan, MF_SEPARATOR, 0, NULL);
    }
    
    AppendMenu(hLan, MF_STRING | MF_ENABLED | MF_UNCHECKED, IDC_JEZYK1, Jezyk::pszMenu[51]);
    
    FindClose(hFile);
    
    SetLastError(_ERROR_SUCCESS);
    return hMenu;
}

HMENU CreateContextMenu(HINSTANCE hThis)
{
    HMENU hContext = GetSubMenu(LoadMenu(hThis, "ContextMenu"), 0);
    if(!hContext) return NULL;
    
    int i = 0, k = Jezyk::iContextBegin;
    char pszText[256];
    
    MENUITEMINFO mii; ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;
    mii.dwTypeData = pszText;
    mii.cch = 256;
    
    while(GetMenuItemInfo(hContext, i, TRUE, &mii))
    {
        mii.cch = 256;
        mii.dwTypeData = pszText;
        
        if(mii.fType == MFT_STRING)
        {
            lstrcpy(pszText, Jezyk::pszMenu[k]);
            SetMenuItemInfo(hContext, i, TRUE, &mii);
            k++;
        }
        
        i++;
    }
    
    return hContext;
}

HWND CreateToolWindow(HWND hParent, HINSTANCE hThis)
{
    HWND hwnd = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hParent, (HMENU)ID_TOOLBAR, hThis, NULL);
    if(!hwnd)
    {
        SetLastError(_ERROR_NOTCREATED);
        return NULL;
    }
    
    HICON hIcon = NULL;
    HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32 | ILC_MASK, 1, 1);
    
    char pszIcons[10][16] =
    {
        "NEW_ICON", "LOAD_ICON", "SAVE_ICON",
        "ADD_ICON", "EDIT_ICON", "DELETE_ICON",
        "OPEN_ICON", "BACK_ICON", "UP_ICON", "DOWN_ICON"
    };
    
    for(int i = 0; i < 10; i++)
    {
        hIcon = LoadIcon(hThis, pszIcons[i]);
        ImageList_AddIcon(hImageList, hIcon);
        DestroyIcon(hIcon);
    }
    
    SendMessage(hwnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
    
    int iItems = 13;
    TBBUTTON tbb[iItems]; ZeroMemory(&tbb, iItems * sizeof(TBBUTTON));
    int iButton[] = 
    {
        IDC_NOWA, IDC_WCZYTAJ, IDC_ZAPISZ, 0,
        IDC_DODAJ, IDC_EDYTUJ, IDC_USUN, 0,
        IDC_OTWORZ, IDC_WSTECZ, 0,
        IDC_GORA, IDC_DOL
    };
    
    for(int i = 0, j = 0; i < iItems; i++)
    {
        tbb[i].fsState = TBSTATE_ENABLED;
        tbb[i].iString = 0;
        tbb[i].iBitmap = (iButton[i] != 0) ? j++ : 0;
        tbb[i].fsStyle = (iButton[i] != 0) ? TBSTYLE_BUTTON : TBSTYLE_SEP;
        tbb[i].idCommand = iButton[i];
    }
    
    SendMessage(hwnd, TB_ADDBUTTONS, (WPARAM)iItems, (LPARAM)tbb);
    
    SetLastError(_ERROR_SUCCESS);
    return hwnd;   
}

HWND CreateStatusWindow(HWND hParent, HINSTANCE hThis)
{
    HWND hwnd = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hParent, (HMENU)ID_STATUS, hThis, NULL);
    if(!hwnd)
    {
        SetLastError(_ERROR_NOTCREATED);
        return NULL;
    }
    
    int fields[3];
    fields[0] = 120;
    fields[1] = 240;
    fields[2] = -1;
    
    SendMessage(hwnd, SB_SETPARTS, 3, (LPARAM)(LPINT)fields);
    
    SetLastError(_ERROR_SUCCESS);
    return hwnd;
}    

HWND CreateMdiWindow(int x, int y, int cx, int cy, HWND hParent, HINSTANCE hThis)
{
    HMENU hMenu = GetMenu(hParent);
    
    CLIENTCREATESTRUCT ccs;
    ccs.hWindowMenu = GetSubMenu(hMenu, 4);
    ccs.idFirstChild = ID_WINDOW;
    
    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "mdiclient", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, x, y, cx, cy, hParent, (HMENU)ID_MDI, hThis, (LPVOID)&ccs);
    if(!hwnd)
    {
        SetLastError(_ERROR_NOTCREATED);
        return NULL;
    }            
    
    SetLastError(_ERROR_SUCCESS);
    return hwnd;
}

HWND CreateMdiListWindow(int x, int y, int cx, int cy, HWND hParent, HINSTANCE hThis)
{
    HWND hwnd = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, x, y, cx, cy, hParent, (HMENU)ID_WIDOK, hThis, NULL);
    
    if(!hwnd)
    {
        SetLastError(_ERROR_NOTCREATED);
        return NULL;
    }
    
    WNDPROC wProc = SubclassWindow(hwnd, ListViewProc);
    SendMessage(hwnd, WM_INITDIALOG, 0, (LPARAM)wProc);
    
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    lvc.fmt = 0;
    
    lvc.pszText = Jezyk::pszColumn[0];
    lvc.iSubItem = 0;
    lvc.cx = 230;
    ListView_InsertColumn(hwnd, 0, &lvc);
    
    lvc.pszText = Jezyk::pszColumn[1];
    lvc.iSubItem = 1;
    lvc.cx = 80;
    ListView_InsertColumn(hwnd, 1, &lvc);
    
    lvc.pszText = Jezyk::pszColumn[2];
    lvc.iSubItem = 2;
    lvc.cx = 80;
    ListView_InsertColumn(hwnd, 2, &lvc);
    
    lvc.pszText = Jezyk::pszColumn[4];
    lvc.iSubItem = 3;
    lvc.cx = 80;
    ListView_InsertColumn(hwnd, 3, &lvc);
    
    HICON hIcon = NULL;
    HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32 | ILC_MASK, 1, 1);
    
    hIcon = LoadIcon(hThis, "AUTOR_ICON");
    ImageList_AddIcon(hImageList, hIcon);
    DestroyIcon(hIcon);

    hIcon = LoadIcon(hThis, "ALBUM_ICON");
    ImageList_AddIcon(hImageList, hIcon);
    DestroyIcon(hIcon);
    
    hIcon = LoadIcon(hThis, "AUDIO_ICON");
    ImageList_AddIcon(hImageList, hIcon);
    DestroyIcon(hIcon);
    
    ListView_SetImageList(hwnd, hImageList, LVSIL_SMALL);
    
    ListView_SetTextColor(hwnd, Opcje::GetTextColor1());
    ListView_SetTextBkColor(hwnd, Opcje::GetBkColor1());
    ListView_SetBkColor(hwnd, Opcje::GetBkColor1());
                            
    LOGFONT lFont; Opcje::GetFont1(&lFont);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)CreateFontIndirect(&lFont), TRUE);
    
    if(Opcje::IsMeshEnabled())
        ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    else
        ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);
    
    SetLastError(_ERROR_SUCCESS);
    return hwnd;
}

HWND CreateProgressBar(HINSTANCE hThis)
{
    RECT rect;
    SendDlgItemMessage(hOkna, ID_STATUS, SB_GETRECT, 1, (LPARAM)&rect);
    
    HWND hProgress = CreateWindowEx(WS_EX_TOPMOST, PROGRESS_CLASS, 0, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, GetDlgItem(hOkna, ID_STATUS), 0, GetModuleHandle(NULL), NULL);
    if(!hProgress) return NULL;
                        
    SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
    SendMessage(hProgress, PBM_SETSTEP, 1, 0);
    
    return hProgress;
}
