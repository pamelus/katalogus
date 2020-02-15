bool Recent::Inicjuj()
{
    for(int i = 0; i < 5; i++)
        pszFiles[i] = NULL;
    
    int size = 0;
    
    RKey rKey;
    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_READ);
    
    for(int i = 0; i < 5; i++)
    {
        char pszName[32];
        wsprintf(pszName, "pszRecent%i", i);
        
        size = rKey.PobierzRozmiar(pszName);
        if(size > 1)
        {
            pszFiles[i] = new char[size];
            rKey.WczytajRekordC(pszName, pszFiles[i], size);
        }
    }
        
    rKey.Zamknij();
    
    return true;
}

bool Recent::Zamknij()
{    
    RKey rKey;
    rKey.Otworz(HKEY_LOCAL_MACHINE, HKEY_MYKEY, KEY_WRITE);
    
    for(int i = 0; i < 5; i++)
    {
        char pszName[32];
        wsprintf(pszName, "pszRecent%i", i);
        
        if(pszFiles[i])
        {
            rKey.ZapiszRekordC(pszName, pszFiles[i]);
            delete [] pszFiles[i];
            pszFiles[i] = NULL;
        }
        else
        {
            rKey.ZapiszRekordC(pszName, "");
        }
    }
    
    rKey.Zamknij();
    
    return true;
}

bool Recent::DodajPlik(const char * pszFile)
{
    for(int i = 0; i < 5; i++)
    {
        if(lstrcmp(pszFiles[i], pszFile) == 0)
            Recent::UsunPlik(pszFile);
    }
    
    if(pszFiles[4]) delete [] pszFiles[4];
    pszFiles[4] = NULL;
    
    for(int i = 4; i > 0; i--)
    {
        pszFiles[i] = pszFiles[i - 1];
    }
    
    pszFiles[0] = new char[lstrlen(pszFile) + 1];
    lstrcpy(pszFiles[0], pszFile);
        
    return true;
}

bool Recent::UsunPlik(const char * pszFile)
{
    for(int i = 0; i < 5; i++)
    {
        if(lstrcmp(pszFiles[i], pszFile) == 0)
        {
            delete [] pszFiles[i];
            pszFiles[i] = NULL;
            
            for(int j = i; j < 4; j++)
            {
                pszFiles[j] = pszFiles[j + 1];
                pszFiles[j + 1] = NULL;
            }
        }
    }
    return true;
}

bool Recent::PobierzPlik(int iItem, char * pszFile, int iSize)
{
    if(iItem < 0 || iItem > 4) return false;
    if(!pszFiles[iItem]) return false;
    
    lstrcpyn(pszFile, pszFiles[iItem], iSize);
    
    return true;
}

bool Recent::UaktualnijMenu(HMENU hMenu)
{
    RemoveMenu(hMenu, IDC_RECENT1, MF_BYCOMMAND);
    RemoveMenu(hMenu, IDC_RECENT2, MF_BYCOMMAND);
    RemoveMenu(hMenu, IDC_RECENT3, MF_BYCOMMAND);
    RemoveMenu(hMenu, IDC_RECENT4, MF_BYCOMMAND);
    RemoveMenu(hMenu, IDC_RECENT5, MF_BYCOMMAND);
    
    bool done = false;
    
    for(int i = 0; i < 5; i++)
    {
        if(pszFiles[i])
        {
            char pszString[25];
            
            PathCompactPathEx(pszString, pszFiles[i], 24, 0);
            
            InsertMenu(hMenu, 7 + i, MF_BYPOSITION | MF_STRING, IDC_RECENT1 + i, pszString);
            
            done = true;
        }
    }
    
    if(!done)
    {
        InsertMenu(hMenu, 7, MF_BYPOSITION | MF_STRING | MF_GRAYED, IDC_RECENT1, "(puste)");
    }
    
    return true;
}
