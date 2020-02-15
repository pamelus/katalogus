bool Jezyk::Inicjuj(const char * pszFile)
{
    for(int i = 0; i < iMaxMenu; i++)
    {
        pszAccel[i] = NULL;
        pszMenu[i] = NULL;
    }
    
    for(int i = 0; i < iMaxDialog; i++)
    {
        pszDialog[i] = NULL;
    }
    
    for(int i = 0; i < iMaxColumn; i++)
    {
        pszColumn[i] = NULL;
    }
    
    for(int i = 0; i < iMaxError; i++)
    {
        pszError[i] = NULL;
    }
    
    for(int i = 0; i < iMaxInne; i++)
    {
        pszInne[i] = NULL;
    }
    
    for(int i = 0; i < iMaxAutor; i++)
    {
        pszAutor[i] = NULL;
    }
        
    pszAccel[1]  = AccelAlloc("Ctrl + N");         pszAccel[2]  = AccelAlloc("Ctrl + O");
    pszAccel[3]  = AccelAlloc("Ctrl + S");         pszAccel[4]  = AccelAlloc("Ctrl + Shift + S");
    pszAccel[5]  = AccelAlloc("Ctrl + W");         pszAccel[7]  = AccelAlloc("Alt + F4");
    pszAccel[9]  = AccelAlloc("Enter");            pszAccel[10] = AccelAlloc("Backspace");
    pszAccel[11] = AccelAlloc("Ctrl + D");         pszAccel[12] = AccelAlloc("F2");
    pszAccel[13] = AccelAlloc("Delete");           pszAccel[14] = AccelAlloc("Ctrl + Up");
    pszAccel[15] = AccelAlloc("Ctrl + Down");      pszAccel[20] = AccelAlloc("Ctrl + F");
    pszAccel[21] = AccelAlloc("Ctrl + Shift + F"); pszAccel[23] = AccelAlloc("F8");
    pszAccel[26] = AccelAlloc("F6");               pszAccel[27] = AccelAlloc("F7");
    pszAccel[28] = AccelAlloc("F5");               pszAccel[34] = AccelAlloc("F1");
    pszAccel[37] = AccelAlloc("Enter");            pszAccel[38] = AccelAlloc("Backspace");
    pszAccel[39] = AccelAlloc("Ctrl + D");         pszAccel[40] = AccelAlloc("F2");
    pszAccel[41] = AccelAlloc("Delete");           pszAccel[42] = AccelAlloc("Ctrl + Up");
    pszAccel[43] = AccelAlloc("Ctrl + Down");      pszAccel[47] = AccelAlloc("Ctrl + F");
    pszAccel[48] = AccelAlloc("Ctrl + Shift + F"); pszAccel[49] = AccelAlloc("F6");
    pszAccel[50] = AccelAlloc("F7");
    
    FILE * pFile = fopen(pszFile, "rb");
    if(!pFile)
    {
        switch(GetLastError())
        {
            case ERROR_ACCESS_DENIED:
            {
                SetLastError(_ERROR_LANFILEACCESSDENIED);
                break;
            }
            default:
            {
                SetLastError(_ERROR_LANFILENOTOPENED);
                break;
            }
        }
        lstrcpy(pszFileName, "");
        return false;
    }
    
    lstrcpy(pszFileName, pszFile);
 
    char pszTemp[256];
    for(int i = 0; i < iMaxMenu; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszMenu[i] = new char[lstrlen(pszTemp) + lstrlen(pszAccel[i]) + 1];
        lstrcpy(pszMenu[i], pszTemp);
        lstrcat(pszMenu[i], pszAccel[i]);
    }
    
    for(int i = 0; i < iMaxDialog; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszDialog[i] = new char[lstrlen(pszTemp) + 1];
        lstrcpy(pszDialog[i], pszTemp);
    }
    
    for(int i = 0; i < iMaxColumn; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszColumn[i] = new char[lstrlen(pszTemp) + 1];
        lstrcpy(pszColumn[i], pszTemp);
    }
    
    for(int i = 0; i < iMaxError; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszError[i] = new char[lstrlen(pszTemp) + 1];
        lstrcpy(pszError[i], pszTemp);
    }
    
    for(int i = 0; i < iMaxInne; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszInne[i] = new char[lstrlen(pszTemp) + 1];
        lstrcpy(pszInne[i], pszTemp);
    }
    
    for(int i = 0; i < iMaxAutor; i++)
    {
        fread(pszTemp, 256, 1, pFile);
        pszAutor[i] = new char[lstrlen(pszTemp) + 1];
        lstrcpy(pszAutor[i], pszTemp);
    }
    
    fclose(pFile);
    
    return true;
}

bool Jezyk::Zamknij()
{
    for(int i = 0; i < iMaxMenu; i++)
    {
        if(pszMenu[i]) delete [] pszMenu[i];
        pszMenu[i] = NULL;
        
        if(pszAccel[i]) delete [] pszAccel[i];
        pszAccel[i] = NULL;
    }
    
    for(int i = 0; i < iMaxDialog; i++)
    {
        if(pszDialog[i]) delete [] pszDialog[i];
        pszDialog[i] = NULL;
    }
    
    for(int i = 0; i < iMaxColumn; i++)
    {
        if(pszColumn[i]) delete [] pszColumn[i];
        pszColumn[i] = NULL;
    }
    
    for(int i = 0; i < iMaxError; i++)
    {
        if(pszError[i]) delete [] pszError[i];
        pszError[i] = NULL;
    }
    
    for(int i = 0; i < iMaxInne; i++)
    {
        if(pszInne[i]) delete [] pszInne[i];
        pszInne[i] = NULL;
    }
    
    for(int i = 0; i < iMaxAutor; i++)
    {
        if(pszAutor[i]) delete [] pszAutor[i];
        pszAutor[i] = NULL;
    }
    
    return true;
}

bool Jezyk::Domyslne(HINSTANCE hThis)
{
    HMENU hMenu = LoadMenu(hThis, "MainMenu");
    char pszText[256];
    
    int i = 0, j = 0, k = 0;
    
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
            pszMenu[k] = new char[lstrlen(pszText) + lstrlen(pszAccel[k]) + 1];
            lstrcpy(pszMenu[k], pszText);
            lstrcat(pszMenu[k], pszAccel[k]);
            k++;
        }
            
        HMENU hSub = GetSubMenu(hMenu, i); j = 0;
        while(GetMenuItemInfo(hSub, j, TRUE, &mii))
        {
            mii.cch = 256;
            mii.dwTypeData = pszText;
            
            if(mii.fType == MFT_STRING)
            {
                pszMenu[k] = new char[lstrlen(pszText) + lstrlen(pszAccel[k]) + 1];
                lstrcpy(pszMenu[k], pszText);
                lstrcat(pszMenu[k], pszAccel[k]);
                k++;
            }
            
            j++;
        }
        
        i++;
    }
    
    DestroyMenu(hMenu);
    hMenu = GetSubMenu(LoadMenu(hThis, "ContextMenu"), 0);
    
    i = 0; while(GetMenuItemInfo(hMenu, i, TRUE, &mii))
    {
        mii.cch = 256;
        mii.dwTypeData = pszText;
        
        if(mii.fType == MFT_STRING)
        {
            pszMenu[k] = new char[lstrlen(pszText) + lstrlen(pszAccel[k]) + 1];
            lstrcpy(pszMenu[k], pszText);
            lstrcat(pszMenu[k], pszAccel[k]);
            k++;
        }
        
        i++;
        
    }
    
    pszMenu[51] = new char[lstrlen("Ustaw domyœlne") + 1];
    lstrcpy(pszMenu[51], "Ustaw domyœlne");
    
    i = 0;
    char pszTemp[256];
    for(int j = 0; j < iMaxDialog; j++, i++)
    {
        if(LoadString(hThis, i, pszTemp, 256) > 0)
        {
           pszDialog[j] = new char[lstrlen(pszTemp) + 1];
           lstrcpy(pszDialog[j], pszTemp); 
        }
    }
    
    for(int j = 0; j < iMaxColumn; j++, i++)
    {
        if(LoadString(hThis, i, pszTemp, 256) > 0)
        {
           pszColumn[j] = new char[lstrlen(pszTemp) + 1];
           lstrcpy(pszColumn[j], pszTemp); 
        }
    }
    
    for(int j = 0; j < iMaxError; j++, i++)
    {
        if(LoadString(hThis, i, pszTemp, 256) > 0)
        {
           pszError[j] = new char[lstrlen(pszTemp) + 1];
           lstrcpy(pszError[j], pszTemp); 
        }
    }
    
    for(int j = 0; j < iMaxInne; j++, i++)
    {
        if(LoadString(hThis, i, pszTemp, 256) > 0)
        {
           pszInne[j] = new char[lstrlen(pszTemp) + 1];
           lstrcpy(pszInne[j], pszTemp); 
        }
    }
    
    for(int j = 0; j < iMaxAutor; j++, i++)
    {
        if(LoadString(hThis, i, pszTemp, 256) > 0)
        {
           pszAutor[j] = new char[lstrlen(pszTemp) + 1];
           lstrcpy(pszAutor[j], pszTemp); 
        }
    }
    
    return true;              
}
