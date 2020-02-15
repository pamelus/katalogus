LRESULT CALLBACK MainDbProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HMENU hContext = NULL;
    static HWND hTip = NULL;
    static HINSTANCE hThis = NULL;
    
    static bool isTipEnabled = true;
    
    switch(message)
    {
        case WM_CREATE:
        {                        
            CREATESTRUCT * cs = (LPCREATESTRUCT)lParam;
            hThis = cs->hInstance;
            
            RECT rect; GetClientRect(hwnd, &rect);
            HWND hWidok = CreateMdiListWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hwnd, hThis);
            if(!hWidok)
            {
                ObslugaBledu();
                return -1;
            }
            
            Katalog * katalog = new Katalog;
            if(!katalog)
            {
                SetLastError(_ERROR_NOTCREATED);
                ObslugaBledu();
                return -1;
            }
            
            SetWindowLong(hwnd, GWL_USERDATA, (int)katalog);
            
            if(((LPMDICREATESTRUCT)cs->lpCreateParams)->lParam)
                if(SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_WCZYTAJ, 0), ((LPMDICREATESTRUCT)cs->lpCreateParams)->lParam) != 0)
                    return -1;
                    
            if(!hContext)
            {
                hContext = CreateContextMenu(hThis);
            }
            
            return 0;
        }
        case WM_CLOSE:
        {
            SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
            
            Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
            
            if(katalog->Modyfikacja())
            {
                char pszMessage[1024];
                char pszCaption[256];
                
                GetWindowText(hwnd, pszCaption, 256);
                wsprintf(pszMessage, Jezyk::pszDialog[130], pszCaption);
                
                int code = MessageBox(hwnd, pszMessage, pszProgramName, MB_YESNOCANCEL | MB_ICONQUESTION);
                switch(code)
                {
                    case IDCANCEL: return -1; break;
                    case IDNO: break;
                    case IDYES:
                    {
                        if(SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_ZAPISZ, 0), 0) != 0)
                        {
                            SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                            return -1;
                        }
                    }
                }          
            }
            
            SendMessage(hwnd, WM_TIPENABLE, 0, 0);
            break;
        }
        case WM_DESTROY:
        {           
            Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
            if(katalog) delete katalog;
            
            if(!(HWND)SendDlgItemMessage(hOkna, ID_MDI, WM_MDIGETACTIVE, 0, 0))
            {
                DestroyMenu(hContext);
                hContext = NULL;
            }
            
            return 0;
        }
        case WM_SIZING:
        {
            RECT * rect = (LPRECT)lParam;
            if((rect->bottom - rect->top) < 150) rect->bottom = rect->top + 150;
            if((rect->right - rect->left) < 300) rect->right = rect->left + 300;
            return 0;
        }
        case WM_SIZE:
        {
            DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
            if(dwStyle & WS_MAXIMIZE)
                SetWindowLong(GetDlgItem(hwnd, ID_WIDOK), GWL_EXSTYLE, WS_EX_ACCEPTFILES);
            else
                SetWindowLong(GetDlgItem(hwnd, ID_WIDOK), GWL_EXSTYLE, WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES);
                        
            int cx = LOWORD(lParam);
            int cy = HIWORD(lParam);
            
            MoveWindow(GetDlgItem(hwnd, ID_WIDOK), 0, 0, cx, cy, true);
            break;
        }
        case WM_SETFOCUS:
        {
            SetFocus(GetDlgItem(hwnd, ID_WIDOK));
            return 0;
        }
        case WM_DROPFILES:
        {
            SetMessageExtraInfo((LPARAM)(HDROP)wParam);
            SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_SKANUJ, 0), 0);
            SetMessageExtraInfo(0);
            return 0;
        }
        case WM_TIPENABLE:
        {
            isTipEnabled = true;
            return 0;
        }
        case WM_TIPDISABLE:
        {
            SendMessage(hwnd, WM_TIPDESTROY, 0, 0);
            isTipEnabled = false;
            return 0;
        }
        case WM_TIPCREATE:
        {
            if(!Opcje::IsCommentEnabled()) return 0;
            if(!isTipEnabled) return 0;
            if(GetFocus() != GetDlgItem(hwnd, ID_WIDOK)) return 0;
            if(hTip) return 0;
            
            HWND hWidok = GetDlgItem(hwnd, ID_WIDOK);
            RECT rcWidok; GetWindowRect(hWidok, &rcWidok);
            POINT p; GetCursorPos(&p);
            
            if(PtInRect(&rcWidok, p))
            {
                Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
                int iItem = -1;
                
                RECT rcItem;
                ScreenToClient(hWidok, &p);
                
                for(int i = 0; i < ListView_GetItemCount(hWidok); i++)
                {
                    ListView_GetItemRect(hWidok, i, &rcItem, LVIR_BOUNDS);
                    if(PtInRect(&rcItem, p))
                    {
                        iItem = i;
                    }
                }
                
                if(iItem < 0) return 0;
                
                DANE dane; katalog->db->PobierzDane(iItem, &dane);
                if(dane.opis[0] == 0) lstrcpyn(dane.opis, Jezyk::pszInne[5], 256);
                
                hTip = CreateWindowEx(WS_EX_TOPMOST, "tipwindow", NULL, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, 0, hThis, NULL); 
                SendMessage(hTip, WM_SETTEXT, 0, (LPARAM)dane.opis);
                ShowWindow(hTip, SW_SHOW);
                
                SetFocus(hwnd);
            }
            
            return 0;
        }
        case WM_TIPDESTROY:
        {
            DestroyWindow(hTip);
            hTip = NULL;
            return 0;
        }
        case WM_MDIACTIVATE:
        {
            HWND hActive = (HWND)lParam;
            if(hActive)
            {
                if(hActive == hwnd)
                {
                    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
                    if(dwStyle & WS_MAXIMIZE)
                        SetWindowLong(GetDlgItem(hwnd, ID_WIDOK), GWL_EXSTYLE, WS_EX_ACCEPTFILES);
                    else
                        SetWindowLong(GetDlgItem(hwnd, ID_WIDOK), GWL_EXSTYLE, WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES);       
                        
                    Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
                
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], katalog->db->PobierzLiczbePozycji());
                    char pszPath[1024]; katalog->db->PobierzSciezke(pszPath, 1024);
                    char pszStatus[256]; if(katalog->Modyfikacja()) lstrcpyn(pszStatus, Jezyk::pszInne[0], 256); else lstrcpy(pszStatus, "");
                
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)pszPath);
                                        
                    EnableMainMenu(true);
                    if(ListView_GetNextItem(GetDlgItem(hwnd, ID_WIDOK), (UINT)-1, LVNI_SELECTED) < 0) EnablePositionMenu(hContext, false);
                    else EnablePositionMenu(hContext, true);
                }
            }
            else
            {
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)"");
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)"");
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)"");
                
                EnableMainMenu(false);
                EnablePositionMenu(hContext, false);
            }    
            return 0;
        }
        case WM_UPDATELANGUAGE:
        {
            Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
            HWND hWidok = GetDlgItem(hwnd, ID_WIDOK);
            
            if((bool)wParam)
            {
                DestroyMenu(hContext);
                hContext = CreateContextMenu(hThis);
                
                char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], katalog->db->PobierzLiczbePozycji());
                char pszPath[1024]; katalog->db->PobierzSciezke(pszPath, 1024);
                char pszStatus[256]; if(katalog->Modyfikacja()) lstrcpyn(pszStatus, Jezyk::pszInne[0], 256); else lstrcpy(pszStatus, "");
                
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)pszPath);
            
                if(ListView_GetNextItem(GetDlgItem(hwnd, ID_WIDOK), (UINT)-1, LVNI_SELECTED) < 0) EnablePositionMenu(hContext, false);
                else EnablePositionMenu(hContext, true);
            }
            
            LV_COLUMN lvc; ZeroMemory(&lvc, sizeof(LV_COLUMN));
            lvc.mask = LVCF_TEXT | LVCF_SUBITEM;
            
                                
            lvc.pszText = Jezyk::pszColumn[0];
            lvc.iSubItem = 0;
            ListView_SetColumn(hWidok, 0, &lvc);
                                
            lvc.pszText = Jezyk::pszColumn[1];
            lvc.iSubItem = 1;
            ListView_SetColumn(hWidok, 1, &lvc);
                                
            lvc.pszText = Jezyk::pszColumn[2];
            lvc.iSubItem = 2;
            ListView_SetColumn(hWidok, 2, &lvc);
                                
            lvc.pszText = Jezyk::pszColumn[4];
            lvc.iSubItem = 3;
            ListView_SetColumn(hWidok, 3, &lvc);
            
            DANE dane;
            for(int i = 0; i < katalog->db->PobierzLiczbePozycji(); i++)
            {
                katalog->db->PobierzDane(i, &dane);
                char pszOpis[16]; wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                ListView_SetItemText(hWidok, i, 2, pszOpis);
            }
            
            return 0;
        }
        case WM_TIMER:
        {
            switch(LOWORD(wParam))
            {
                case IDTM_SERIAL:
                {
                    KillTimer(hwnd, IDTM_SERIAL);
                    SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_DODAJ, 0), 0);
                    return 0;
                }
            }
            break;
        }
        case WM_NOTIFY:
        {
            NMHDR * nmhdr = (LPNMHDR)lParam;
            switch(nmhdr->code)
            {   
                case LVN_COLUMNCLICK:
                {                    
                    NMLISTVIEW * nmlv = (LPNMLISTVIEW)lParam;
                    switch(nmlv->iSubItem)
                    {
                        case 0: SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_SZEREGUJNAZWA, 0), 0); break;
                        case 1: SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_SZEREGUJNOSNIK, 0), 0); break;
                        case 2: SendMessage(hwnd, WM_COMMAND, MAKELONG(IDC_SZEREGUJOPIS, 0), 0); break;
                    }
                    
                    return 0;
                }
                case LVN_ITEMCHANGED:
                {
                    NMLISTVIEW * nmlv = (LPNMLISTVIEW)lParam;
                    if(nmlv->uChanged & LVIF_STATE)
                    {
                        if(nmlv->uNewState & LVIS_SELECTED) EnablePositionMenu(hContext, true);
                        else EnablePositionMenu(hContext, false);                    
                    }
                    return 0;
                }
                case LVN_DELETEALLITEMS:
                {
                    if(hwnd == (HWND)SendDlgItemMessage(hOkna, ID_MDI, WM_MDIGETACTIVE, 0, 0))
                        EnablePositionMenu(hContext, false);
                    return TRUE;
                }
            }
            break;
        }
        case WM_COMMAND:
        {                        
            Katalog * katalog = (Katalog *)GetWindowLong(hwnd, GWL_USERDATA);
            HWND hWidok = GetDlgItem(hwnd, ID_WIDOK);
            switch(LOWORD(wParam))
            {
                case IDC_WCZYTAJ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    char pszFile[1024] = "";
                    char pszTitle[256] = "";
                    char pszFilter[256]; CreateFilter(pszFilter, 0);
                    
                    if(((char *)lParam)[0] == '0')
                    {
                        OPENFILENAME ofn;
                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hwnd;
                        ofn.hInstance = hThis;
                        ofn.lpstrFilter = pszFilter;
                        ofn.lpstrCustomFilter = NULL;
                        ofn.nMaxCustFilter = 0;
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFile = pszFile;
                        ofn.nMaxFile = 1024;
                        ofn.lpstrFileTitle = pszTitle;
                        ofn.nMaxFileTitle = 256;
                        ofn.lpstrInitialDir = NULL;
                        ofn.lpstrDefExt = "kdb";
                        ofn.lCustData = 0;
                        ofn.lpstrTitle = Jezyk::pszDialog[7];
                        ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                        
                        if(!GetOpenFileName(&ofn))
                        {
                            SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                            return -1;
                        }
                    }
                    else
                    {
                        lstrcpyn(pszFile, (char *)lParam, 1024);
                        lstrcpyn(pszTitle, PathFindFileName(pszFile), 256);
                    }
                    
                    PathAddExtension(pszFile, ".kdb");
                    katalog->UstawPlik(pszFile);
                    Recent::DodajPlik(pszFile);
                    
                    DBFILETAG dbf;
                    dbf.pszFile = pszFile;
                    dbf.katalog = katalog;
                    dbf.hProgress = CreateProgressBar(hThis);
                    dbf.koniec = false;
                    dbf.result = false;
                    dbf.error = 0;
                    
                    EnableWindow(hOkna, false);
                    
                    DWORD value = 0;
                    CreateThread(NULL, 0, LoadThread, (LPVOID)&dbf, 0, &value);
                    
                    MSG msg;
                    while(!dbf.koniec)
                        PetlaSterujaca(&msg);
                        
                    EnableWindow(hOkna, true);
                    DestroyWindow(dbf.hProgress);
                        
                    if(!dbf.result)
                    {
                        SetLastError(dbf.error);
                        ObslugaBledu();                  
                        
                        Recent::UsunPlik(pszFile);
                        Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                        
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                        
                    PathRemoveExtension(pszTitle);
                    SetWindowText(hwnd, pszTitle);
                        
                    int iCount = katalog->db->PobierzLiczbePozycji();
                        
                    DANE dane[iCount];
                    for(int i = 0; i < iCount; i++)
                    katalog->db->PobierzDane(i, &dane[i]);
                    
                    ListView_AddItems(hWidok, katalog->PobierzPoziom(), dane, iCount);
                    ListView_SetItemState(hWidok, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, 0, false);
                    
                    Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                        
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], iCount);
                    char pszPath[1024]; katalog->db->PobierzSciezke(pszPath, 1024);
                    
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)pszPath);
                    SetFocus(hwnd);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_ZAPISZ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    char pszFile[1024] = "";
                    char pszTitle[256] = "";
                    if(!katalog->PobierzPlik(pszFile, 1024))
                    {
                        char pszFilter[256]; CreateFilter(pszFilter, 0);
                        
                        OPENFILENAME ofn;
                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hwnd;
                        ofn.hInstance = hThis;
                        ofn.lpstrFilter = pszFilter;
                        ofn.lpstrCustomFilter = NULL;
                        ofn.nMaxCustFilter = 0;
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFile = pszFile;
                        ofn.nMaxFile = 1024;
                        ofn.lpstrFileTitle = pszTitle;
                        ofn.nMaxFileTitle = 256;
                        ofn.lpstrInitialDir = NULL;
                        ofn.lpstrDefExt = "kdb";
                        ofn.lCustData = 0;
                        ofn.lpstrTitle = Jezyk::pszDialog[8];
                        ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                        if(GetSaveFileName(&ofn))
                        {
                            PathAddExtension(pszFile, ".kdb");
                            katalog->UstawPlik(pszFile);
                            Recent::DodajPlik(pszFile);
                            
                            PathRemoveExtension(pszTitle);
                            SetWindowText(hwnd, pszTitle);
                        }
                        else
                        {
                            SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                            return -1;
                        }
                    }
                    
                    DBFILETAG dbf;
                    dbf.pszFile = pszFile;
                    dbf.katalog = katalog;
                    dbf.hProgress = CreateProgressBar(hThis);
                    dbf.koniec = false;
                    dbf.result = false;
                    dbf.error = 0;
                    
                    EnableWindow(hOkna, false);
                    
                    DWORD value;
                    CreateThread(NULL, 0, SaveThread, (LPVOID)&dbf, 0, &value);
                    
                    MSG msg;
                    while(!dbf.koniec)
                        PetlaSterujaca(&msg);
                        
                    EnableWindow(hOkna, true);
                    DestroyWindow(dbf.hProgress);
                
                    if(!dbf.result)
                    {             
                        SetLastError(dbf.error);          
                        ObslugaBledu();
                        
                        katalog->UstawPlik(NULL);
                        Recent::UsunPlik(pszFile);
                        Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                        
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                    
                    Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                    
                    katalog->UstawModyfikacja(false);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)"");
                    SetFocus(hwnd);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_ZAPISZJAKO:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    char pszFile[1024] = "";            
                    char pszTitle[256] = "";
                    char pszFilter[256]; CreateFilter(pszFilter, 0);
                    
                    katalog->PobierzPlik(pszFile, 1024);
                        
                    OPENFILENAME ofn;
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hwnd;
                    ofn.hInstance = hThis;
                    ofn.lpstrFilter = pszFilter;
                    ofn.lpstrCustomFilter = NULL;
                    ofn.nMaxCustFilter = 0;
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFile = pszFile;
                    ofn.nMaxFile = 1024;
                    ofn.lpstrFileTitle = pszTitle;
                    ofn.nMaxFileTitle = 256;
                    ofn.lpstrInitialDir = NULL;
                    ofn.lpstrDefExt = "kdb";
                    ofn.lCustData = 0;
                    ofn.lpstrTitle = Jezyk::pszDialog[9];
                    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                    
                    if(GetSaveFileName(&ofn))
                    {
                        PathAddExtension(pszFile, ".kdb");
                        katalog->UstawPlik(pszFile);
                        Recent::DodajPlik(pszFile);
                            
                        PathRemoveExtension(pszTitle);
                        SetWindowText(hwnd, pszTitle);
                        
                        DBFILETAG dbf;
                        dbf.pszFile = pszFile;
                        dbf.katalog = katalog;
                        dbf.hProgress = CreateProgressBar(hThis);
                        dbf.koniec = false;
                        dbf.result = false;
                        dbf.error = 0;
                        
                        EnableWindow(hOkna, false);
                        
                        DWORD value;
                        CreateThread(NULL, 0, SaveThread, (LPVOID)&dbf, 0, &value);
                    
                        MSG msg;
                        while(!dbf.koniec)
                            PetlaSterujaca(&msg);
                        
                        EnableWindow(hOkna, true);
                        DestroyWindow(dbf.hProgress);
                        
                        if(!dbf.result)
                        {
                            SetLastError(dbf.error);
                            ObslugaBledu();
                            
                            katalog->UstawPlik(NULL);
                            Recent::UsunPlik(pszFile);
                            Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                            
                            SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                            return -1;
                        }
                        
                        Recent::UaktualnijMenu(GetSubMenu(GetMenu(hOkna), 0));
                    
                        katalog->UstawModyfikacja(false);
                        SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)"");
                        SetFocus(hwnd);
                        
                    }
                    else
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }    
                case IDC_OTWORZ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    int iItem = ListView_GetNextItem(hWidok, (UINT)-1, LVNI_SELECTED);
                    katalog->db->UstawOtwarty(iItem);
                    if(!katalog->Otworz(iItem))
                    {
                        katalog->db->UstawOtwarty(0);
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    
                    int iCount = katalog->db->PobierzLiczbePozycji();
                    
                    DANE dane[iCount];
                    for(int i = 0; i < iCount; i++)
                        katalog->db->PobierzDane(i, &dane[i]);
                    
                    ListView_DeleteAllItems(hWidok);
                    ListView_AddItems(hWidok, katalog->PobierzPoziom(), dane, iCount);
                    ListView_SetItemState(hWidok, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, 0, false);
                    
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], iCount);
                    char pszPath[1024]; katalog->db->PobierzSciezke(pszPath, 1024);
                
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)pszPath);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_WSTECZ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    if(!katalog->Wstecz())
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    int iItem = katalog->db->PobierzOtwarty();
                    int iCount = katalog->db->PobierzLiczbePozycji();
                    
                    DANE dane[iCount];
                    for(int i = 0; i < iCount; i++)
                        katalog->db->PobierzDane(i, &dane[i]);
                    
                    ListView_DeleteAllItems(hWidok);
                    ListView_AddItems(hWidok, katalog->PobierzPoziom(), dane, iCount);
                    ListView_SetItemState(hWidok, iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, iItem, false);
                    
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], iCount);
                    char pszPath[1024]; katalog->db->PobierzSciezke(pszPath, 1024);
                
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)2, (LPARAM)(LPCSTR)pszPath);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_DODAJ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    DANE dane = { "", "", "" };
                    
                    int code = DialogBoxParam(hThis, "ITEM_DIALOG", hwnd, ItemProc, (LPARAM)&dane);
                    if(code != 0)
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                    
                    if(katalog->db->SprawdzPozycja(dane.nazwa) > -1)
                    {
                        if(Opcje::IsWarningEnabled())
                        {
                            char pszMessage[1024]; wsprintf(pszMessage, Jezyk::pszDialog[132], dane.nazwa);
                            int code = MessageBox(hwnd, pszMessage, pszProgramName, MB_YESNO | MB_ICONQUESTION);
                            if(code == IDNO)
                            {
                                SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                                return -1;
                            }
                        }
                    }
                    
                    katalog->db->DodajPozycja(&dane);
                                                    
                    int iItem = ListView_AddItems(hWidok, katalog->PobierzPoziom(), &dane, 1);
                    
                    ListView_SetItemState(hWidok, iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, iItem, false);
                        
                    katalog->UstawModyfikacja(true);
                    
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], katalog->db->PobierzLiczbePozycji());
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                                    
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                                        
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    
                    int iLevel = katalog->PobierzPoziom();
                    if((iLevel == 0 && Opcje::IsSerialAutor()) || (iLevel == 1 && Opcje::IsSerialAlbum()) || (iLevel == 2 && Opcje::IsSerialAudio()))
                    {
                        SetTimer(hwnd, IDTM_SERIAL, 1, NULL);
                    }    
                    
                    return iItem;
                }
                case IDC_EDYTUJ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    int iItem = ListView_GetNextItem(hWidok, (UINT)-1, LVNI_SELECTED);
                                        
                    DANE dane;
                    
                    if(!katalog->db->PobierzDane(iItem, &dane))
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    
                    int code = DialogBoxParam(hThis, "ITEM_DIALOG", hwnd, ItemProc, (LPARAM)&dane);
                    if(code != 0)
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }    
                    
                    katalog->db->EdytujPozycja(iItem, &dane);
                                                
                    char pszOpis[16]; wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                    char pszIndex[16]; wsprintf(pszIndex, "%i", iItem + 1);
                    
                    ListView_SetItemText(hWidok, iItem, 0, dane.nazwa);
                    ListView_SetItemText(hWidok, iItem, 1, dane.nosnik);
                    ListView_SetItemText(hWidok, iItem, 2, pszOpis);
                    ListView_SetItemText(hWidok, iItem, 3, pszIndex);
                    ListView_SetItemState(hWidok, iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, iItem, false);
                            
                    katalog->UstawModyfikacja(true);
                    
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_USUN:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    int iItem = ListView_GetNextItem(hWidok, (UINT)-1, LVNI_SELECTED);
                    
                    if(!katalog->db->UsunPozycja(iItem))
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    
                    ListView_DeleteItem(hWidok, iItem);
                    int iCount = ListView_GetItemCount(hWidok);
                        
                    for(int i = iItem; i < iCount; i++)
                    {
                        char pszIndex[16]; wsprintf(pszIndex, "%i", i+1);
                        ListView_SetItemText(hWidok, i, 3, pszIndex);
                    }
                        
                    if(iItem == iCount)
                    {
                        ListView_SetItemState(hWidok, iItem - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                        ListView_EnsureVisible(hWidok, iItem - 1, false);
                    }
                    else
                    {
                        ListView_SetItemState(hWidok, iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                        ListView_EnsureVisible(hWidok, iItem, false);
                    }
                        
                    katalog->UstawModyfikacja(true);
                    
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], katalog->db->PobierzLiczbePozycji());
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                                    
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_GORA:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    int iItem = ListView_GetNextItem(hWidok, (UINT)-1, LVNI_SELECTED);
                    
                    if(!katalog->db->PrzeniesWGore(iItem))
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    
                    DANE dane;
                    
                    katalog->db->PobierzDane(iItem - 1, &dane);
                                       
                    char pszOpis[16]; wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                    char pszIndex[16]; wsprintf(pszIndex, "%i", iItem);
                    
                    ListView_SetItemText(hWidok, iItem - 1, 0, dane.nazwa);
                    ListView_SetItemText(hWidok, iItem - 1, 1, dane.nosnik);
                    ListView_SetItemText(hWidok, iItem - 1, 2, pszOpis);
                    ListView_SetItemText(hWidok, iItem - 1, 3, pszIndex);
                    
                    katalog->db->PobierzDane(iItem, &dane);
                    
                    wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                    wsprintf(pszIndex, "%i", iItem + 1);
                    
                    ListView_EnsureVisible(hWidok, iItem, false);
                    ListView_SetItemText(hWidok, iItem, 0, dane.nazwa);
                    ListView_SetItemText(hWidok, iItem, 1, dane.nosnik);
                    ListView_SetItemText(hWidok, iItem, 2, pszOpis);
                    ListView_SetItemText(hWidok, iItem, 3, pszIndex);
                    
                    ListView_SetItemState(hWidok, iItem - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, iItem - 1, false);
                    
                    katalog->UstawModyfikacja(true);
                    
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_DOL:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    int iItem = ListView_GetNextItem(hWidok, (UINT)-1, LVNI_SELECTED);
                    
                    if(!katalog->db->PrzeniesWDol(iItem))
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        ObslugaBledu();
                        return -1;
                    }
                    
                    DANE dane;
                    
                    katalog->db->PobierzDane(iItem + 1, &dane);
                    
                    char pszOpis[16]; wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                    char pszIndex[16]; wsprintf(pszIndex, "%i", iItem);
                    
                    ListView_SetItemText(hWidok, iItem + 1, 0, dane.nazwa);
                    ListView_SetItemText(hWidok, iItem + 1, 1, dane.nosnik);
                    ListView_SetItemText(hWidok, iItem + 1, 2, pszOpis);
                    ListView_SetItemText(hWidok, iItem + 1, 3, pszIndex);
                    
                    katalog->db->PobierzDane(iItem, &dane);
                    
                    wsprintf(pszOpis, "%i %s", lstrlen(dane.opis), Jezyk::pszInne[2]);
                    wsprintf(pszIndex, "%i", iItem + 1);
                    
                    ListView_EnsureVisible(hWidok, iItem, false);
                    ListView_SetItemText(hWidok, iItem, 0, dane.nazwa);
                    ListView_SetItemText(hWidok, iItem, 1, dane.nosnik);
                    ListView_SetItemText(hWidok, iItem, 2, pszOpis);
                    ListView_SetItemText(hWidok, iItem, 3, pszIndex);
                    
                    ListView_SetItemState(hWidok, iItem + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, iItem + 1, false);
                    
                    katalog->UstawModyfikacja(true);
                    
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_SZEREGUJNAZWA:
                case IDC_SZEREGUJNOSNIK:
                case IDC_SZEREGUJOPIS:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    SORTTAG srt;
                    srt.db = katalog->db;
                    srt.hProgress = CreateProgressBar(hThis);
                    srt.code = LOWORD(wParam);
                    srt.result = false;
                    srt.koniec = false;
                    srt.error = 0;
                    
                    EnableWindow(hOkna, false);
                    
                    DWORD value;
                    CreateThread(NULL, 0, SortThread, (LPVOID)&srt, 0, &value);
                    
                    MSG msg;
                    while(!srt.koniec)
                        PetlaSterujaca(&msg);
                    
                    EnableWindow(hOkna, true);
                    DestroyWindow(srt.hProgress);
                    
                    if(!srt.result)
                    {
                        SetLastError(srt.error);
                        ObslugaBledu();
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                    
                    int iCount = katalog->db->PobierzLiczbePozycji();
                    
                    DANE dane[iCount];
                    for(int i = 0; i < iCount; i++)
                        katalog->db->PobierzDane(i, &dane[i]);
                    
                    ListView_DeleteAllItems(hWidok);
                    ListView_AddItems(hWidok, katalog->PobierzPoziom(), dane, iCount);      

                    ListView_SetItemState(hWidok, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                    ListView_EnsureVisible(hWidok, 0, false);
                    
                    katalog->UstawModyfikacja(true);
                    
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
                    SetFocus(hwnd);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_SZUKAJNAZWA:
                case IDC_SZUKAJNOSNIK:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    SEARCHTAG srch;
                    srch.db = (katalog->db->ZamknijPozycje())->ZamknijPozycje();
                    srch.koniec = false;
                    srch.code = LOWORD(wParam);
                    srch.fnd.rslt = NULL;
                    srch.fnd.iRes = 0;
                    srch.fnd.autor = srch.fnd.album = srch.fnd.audio = true;
                    srch.fnd.poziom = 0;
                    
                    lstrcpy(srch.fnd.pszFraza, "");
                    
                    RKey rKey;
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_READ);
                    rKey.WczytajRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAutor" : "SearchMediumAutor", srch.fnd.autor);
                    rKey.WczytajRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAlbum" : "SearchMediumAlbum", srch.fnd.album);
                    rKey.WczytajRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAudio" : "SearchMediumAudio", srch.fnd.audio);
                    rKey.WczytajRekordC((srch.code == IDC_SZUKAJNAZWA) ? "SearchNamePhrase" : "SearchMediumPhrase", srch.fnd.pszFraza, 256);
                    rKey.Zamknij();
                    
                    int code = DialogBoxParam(hThis, "FIND_DIALOG", hwnd, FindProc, (LPARAM)&srch.fnd);
                    if(code != 0)
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }    
                    
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_WRITE);
                    rKey.ZapiszRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAutor" : "SearchMediumAutor", srch.fnd.autor);
                    rKey.ZapiszRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAlbum" : "SearchMediumAlbum", srch.fnd.album);
                    rKey.ZapiszRekordA((srch.code == IDC_SZUKAJNAZWA) ? "SearchNameAudio" : "SearchMediumAudio", srch.fnd.audio);
                    rKey.ZapiszRekordC((srch.code == IDC_SZUKAJNAZWA) ? "SearchNamePhrase" : "SearchMediumPhrase", srch.fnd.pszFraza);
                    rKey.Zamknij();
                    
                    srch.fnd.hProgress = CreateProgressBar(hThis);
                    EnableWindow(hOkna, false);
                    
                    DWORD value;
                    CreateThread(NULL, 0, SearchThread, (LPVOID)&srch, 0, &value);
                    
                    MSG msg;
                    while(!srch.koniec)
                        PetlaSterujaca(&msg);
                    
                    EnableWindow(hOkna, true);
                    DestroyWindow(srch.fnd.hProgress);
                    
                    if(srch.fnd.iRes > 0)
                    {
                        CreateDialogParam(hThis, "RESULT_DIALOG", hwnd, ResultProc, (LPARAM)&srch.fnd);
                        delete [] srch.fnd.rslt;
                    }
                    else
                    {
                        char message[1024];
                        wsprintf(message, Jezyk::pszDialog[133], srch.fnd.pszFraza);
                        MessageBox(hwnd, message, pszProgramName, MB_OK | MB_ICONINFORMATION);
                    }
                    SetFocus(hwnd);
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_SKANUJ:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    char pszFolder[1024] = "";
                    HDROP hDrop = (HDROP)GetMessageExtraInfo();
                    
                    if(!hDrop)
                    {
                        BROWSEINFO bi;
                        bi.hwndOwner = hwnd;
                        bi.pidlRoot = NULL;
                        bi.pszDisplayName = pszFolder;
                        bi.lpszTitle = Jezyk::pszDialog[127];
                        bi.ulFlags = 0;
                        bi.lpfn = NULL;
                        bi.lParam = 0;
                        bi.iImage = 0;
                    
                        ITEMIDLIST * pid = NULL;
                        if(pid = SHBrowseForFolder(&bi))
                        {
                            SHGetPathFromIDList(pid, pszFolder);   
                        }
                        else
                        {
                            return -1;
                        }
                    }    
                    
                    SCANINITTAG sInit;
                    sInit.scn.autor = sInit.scn.album = sInit.scn.audio = true;
                    sInit.scn.yes = sInit.scn.no = false;
                    sInit.scn.hWidok = GetDlgItem(hwnd, ID_WIDOK);
                    sInit.scn.db = katalog->db;
                    sInit.scn.iImage = katalog->PobierzPoziom();
            
                    lstrcpy(sInit.scn.dane.nazwa, "");
                    lstrcpy(sInit.scn.dane.nosnik, "");
                    lstrcpy(sInit.scn.dane.opis, "");
            
                    RKey rKey;
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_READ);
                    rKey.WczytajRekordA("ScanAutor", sInit.scn.autor);
                    rKey.WczytajRekordA("ScanAlbum", sInit.scn.album);
                    rKey.WczytajRekordA("ScanAudio", sInit.scn.audio);
                    rKey.Zamknij();
            
                    int code = DialogBoxParam(hThis, "SCANITEM_DIALOG", hwnd, ScanItemProc, (LPARAM)&sInit.scn);
                    if(code != 0)
                    {
                        SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                        return -1;
                    }
                        
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_WRITE);
                    rKey.ZapiszRekordA("ScanAutor", sInit.scn.autor);
                    rKey.ZapiszRekordA("ScanAlbum", sInit.scn.album);
                    rKey.ZapiszRekordA("ScanAudio", sInit.scn.audio);
                    rKey.Zamknij();
            
                    sInit.pszFolder = pszFolder;
                    sInit.hDrop = hDrop;
                    DialogBoxParam(hThis, "SCAN_DIALOG", hwnd, ScanProc, (LPARAM)&sInit);
                    DragFinish(sInit.hDrop);
            
                    katalog->UstawModyfikacja(true);
                    
                    char pszLiczba[256]; wsprintf(pszLiczba, "%s %i", Jezyk::pszInne[1], katalog->db->PobierzLiczbePozycji());
                    char pszStatus[256]; lstrcpyn(pszStatus, Jezyk::pszInne[0], 256);
                                    
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)0, (LPARAM)(LPCSTR)pszLiczba);
                    SendDlgItemMessage(hOkna, ID_STATUS, SB_SETTEXT, (WPARAM)1, (LPARAM)(LPCSTR)pszStatus);
            
                    SetForegroundWindow(hOkna);
                    SetFocus(GetDlgItem(hwnd, ID_WIDOK));
            
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    
                    return 0;
                }
                case IDC_STATYSTYKA:
                {
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);
                    
                    BazaDanych * db = (katalog->db->ZamknijPozycje())->ZamknijPozycje();
                    
                    STATSTAG scn;
                    scn.iAutor = 0;
                    scn.iAlbum = 0;
                    scn.iAudio = 0;
                    
                    GetWindowText(hwnd, scn.pszDatabase, 256);
                    
                    scn.iAutor = db->PobierzLiczbePozycji();
                    scn.iAlbum = 0;
                    scn.iAudio = 0;
                    
                    for(int i = 0; i < scn.iAutor; i++)
                    {
                        db = db->OtworzPozycje(i);
                        
                        int iTemp1 = db->PobierzLiczbePozycji();
                        scn.iAlbum += iTemp1;
                        
                        for(int j = 0; j < iTemp1; j++)
                        {
                            db = db->OtworzPozycje(j);
                            
                            int iTemp2 = db->PobierzLiczbePozycji();
                            scn.iAudio += iTemp2;
                            
                            db = db->ZamknijPozycje();
                        }
                        db = db->ZamknijPozycje();
                    }
                    
                    CreateDialogParam(hThis, "STATS_DIALOG", hwnd, StatsProc, (LPARAM)&scn);
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_RAPORT:
                {                                        
                    SendMessage(hwnd, WM_TIPDISABLE, 0, 0);                    
                    
                    BazaDanych * db = ((katalog->db)->ZamknijPozycje())->ZamknijPozycje();
                    
                    HPROPSHEETPAGE pages[4];
                    
                    PROPSHEETPAGE page;
                    page.dwSize = sizeof(PROPSHEETPAGE);
                    page.dwFlags = PSP_USETITLE;
                    page.hInstance = GetModuleHandle(NULL);
                    page.pszTemplate = "RWIZARD0_DIALOG";
                    page.pfnDlgProc = Wizard0Proc;
                    page.pszTitle = Jezyk::pszDialog[32];
                    pages[0] = CreatePropertySheetPage(&page);
            
                    page.pszTemplate = "RWIZARD1_DIALOG";
                    page.pfnDlgProc = Wizard1Proc;
                    page.pszTitle = Jezyk::pszDialog[46];
                    pages[1] = CreatePropertySheetPage(&page);
            
                    page.pszTemplate = "RWIZARD2_DIALOG";
                    page.pfnDlgProc = Wizard2Proc;
                    page.pszTitle = Jezyk::pszDialog[54];
                    pages[2] = CreatePropertySheetPage(&page);
                    
                    page.pszTemplate = "RWIZARD3_DIALOG";
                    page.pfnDlgProc = Wizard3Proc;
                    page.pszTitle = Jezyk::pszDialog[59];
                    pages[3] = CreatePropertySheetPage(&page);
              
                    PROPSHEETHEADER head;
                    head.dwSize = sizeof(PROPSHEETHEADER);
                    head.dwFlags = PSH_NOAPPLYNOW | PSH_WIZARD;
                    head.hwndParent = GetParent(hwnd);
                    head.hInstance = GetModuleHandle(NULL);
                    head.nPages = 4;
                    head.nStartPage = 0;
                    head.phpage = pages;
            
                    REPORTINITTAG rpInit; ZeroMemory(&rpInit.rprt, sizeof(REPORTTAG));
                    DWORD dwUklad = 0;
            
                    RKey rKey;
                    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_READ);
                    rKey.WczytajRekordD("ReportFonts", rpInit.rprt.fonts, 6 * sizeof(LOGFONT));
                    rKey.WczytajRekordD("ReportColors", rpInit.rprt.colors, 6 * sizeof(COLORREF));
                    rKey.WczytajRekordD("ReportDana", rpInit.rprt.dane, 3 * sizeof(int));
                    rKey.WczytajRekordB("ReportUklad", dwUklad);
                    rKey.WczytajRekordD("ReportOptions", rpInit.rprt.options, 4 * sizeof(bool));
                    rKey.WczytajRekordD("ReportStop", rpInit.rprt.pszStop, 2 * 16);
                    rKey.Zamknij();
            
                    Wizard0Proc(NULL, WM_SETINFO, (WPARAM)rpInit.rprt.colors, (LPARAM)rpInit.rprt.fonts);
                    Wizard1Proc(NULL, WM_SETINFO, (WPARAM)&dwUklad, (LPARAM)rpInit.rprt.dane);
                    Wizard2Proc(NULL, WM_SETINFO, (WPARAM)0, (LPARAM)rpInit.rprt.pszHeaders);
                    Wizard3Proc(NULL, WM_SETINFO, (WPARAM)rpInit.rprt.options, (LPARAM)rpInit.rprt.pszStop);
            
                    if(PropertySheet(&head) == IDOK)
                    {
                        Wizard0Proc(NULL, WM_GETINFO, (WPARAM)rpInit.rprt.colors, (LPARAM)rpInit.rprt.fonts);
                        Wizard1Proc(NULL, WM_GETINFO, (WPARAM)&dwUklad, (LPARAM)rpInit.rprt.dane);
                        Wizard2Proc(NULL, WM_GETINFO, (WPARAM)0, (LPARAM)rpInit.rprt.pszHeaders);
                        Wizard3Proc(NULL, WM_GETINFO, (WPARAM)rpInit.rprt.options, (LPARAM)rpInit.rprt.pszStop);
            
                        rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_WRITE);
                        rKey.ZapiszRekordD("ReportFonts", rpInit.rprt.fonts, 6 * sizeof(LOGFONT));
                        rKey.ZapiszRekordD("ReportColors", rpInit.rprt.colors, 6 * sizeof(COLORREF));
                        rKey.ZapiszRekordD("ReportDana", rpInit.rprt.dane, 3 * sizeof(int));
                        rKey.ZapiszRekordB("ReportUklad", dwUklad);
                        rKey.ZapiszRekordD("ReportOptions", rpInit.rprt.options, 4 * sizeof(bool));
                        rKey.ZapiszRekordD("ReportStop", rpInit.rprt.pszStop, 2 * 16);
                        rKey.Zamknij();
                        
                        rpInit.rprt.kaskada[0] = (LOWORD(dwUklad) == 1) ? true : false;
                        rpInit.rprt.kaskada[1] = (HIWORD(dwUklad) == 1) ? true : false;
                        
                        char pszFilter[1024]; CreateFilter(pszFilter, 1);
                        
                        OPENFILENAME ofn;
                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hwnd;
                        ofn.hInstance = hThis;
                        ofn.lpstrFilter = pszFilter;
                        ofn.lpstrCustomFilter = NULL;
                        ofn.nMaxCustFilter = 0;
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFile = rpInit.rprt.pszFile;
                        ofn.nMaxFile = 1024;
                        ofn.lpstrFileTitle = NULL;
                        ofn.nMaxFileTitle = 0;
                        ofn.lpstrInitialDir = NULL;
                        ofn.lpstrDefExt = "html";
                        ofn.lCustData = 0;
                        ofn.lpstrTitle = Jezyk::pszDialog[129];
                        ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
                        if(GetSaveFileName(&ofn))
                        {                            
                            PathAddExtension(rpInit.rprt.pszFile, "html");
                            GetWindowText(hwnd, rpInit.rprt.pszTitle, 256);
                            
                            rpInit.hProgress = CreateProgressBar(hThis);
                            rpInit.db = (katalog->db->ZamknijPozycje())->ZamknijPozycje();
                            rpInit.error = 0;
                            rpInit.result = false;
                            rpInit.koniec = false;
                            
                            EnableWindow(hOkna, false);
                            
                            DWORD value;
                            CreateThread(NULL, 0, ReportThread, (LPVOID)&rpInit, 0, &value);
                            
                            MSG msg;
                            while(!rpInit.koniec)
                                PetlaSterujaca(&msg);
                            
                            EnableWindow(hOkna, true);
                            DestroyWindow(rpInit.hProgress);
                            
                            if(!rpInit.result)
                            {
                                SetLastError(rpInit.error);
                                ObslugaBledu();
                            }    
                            
                            SetFocus(hwnd);
                        }
                    }
                    
                    SendMessage(hwnd, WM_TIPENABLE, 0, 0);
                    return 0;
                }
                case IDC_ZAMKNIJ:
                {
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    return 0;
                }
                case IDC_MENU:
                {                    
                    POINT p; GetCursorPos(&p);
                    TrackPopupMenu(hContext, TPM_RIGHTBUTTON, p.x, p.y, 0, hwnd, NULL); 
                    return 0;
                }
            }
            break;
        }
    }
    return DefMDIChildProc(hwnd, message, wParam, lParam);
}
