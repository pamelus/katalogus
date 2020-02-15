Katalog::Katalog():
iMaxPoziom(2)
{
    pszFile = NULL;
    pszPass = NULL;
    poziom = 0;
    modified = false;
    
    db = new BazaDanych(NULL);
}

Katalog::~Katalog()
{
    if(pszFile) delete [] pszFile;
    if(pszPass) delete [] pszPass;
    
    db = (db->ZamknijPozycje())->ZamknijPozycje();
    delete db;
}

bool Katalog::Otworz(int index)
{
    if(poziom == iMaxPoziom)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    db = db->OtworzPozycje(index);
    
    if(GetLastError() == _ERROR_OVERDRIVE)
        return false;
    
    poziom++;
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}
    
bool Katalog::Wstecz()
{
    if(poziom == 0)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    db = db->ZamknijPozycje();
    
    if(GetLastError() == _ERROR_OVERDRIVE)
        return false;
    
    poziom--;
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

bool Katalog::UstawPlik(const char * pszNewFile)
{
    if(pszFile) delete [] pszFile;
    if(pszNewFile)
    {
        pszFile = new char[lstrlen(pszNewFile) + 1];
        lstrcpy(pszFile, pszNewFile);
    }
    else
    {
        pszFile = NULL;
    }
    return true;
}
    
bool Katalog::PobierzPlik(char * pszPlik, int size)
{
    if(!pszFile)
    {
        lstrcpyn(pszPlik, "", size);
        return false;
    }
    
    lstrcpyn(pszPlik, pszFile, size);
    return true;
}

bool Katalog::UstawHaslo(const char * pszNewPass)
{
    if(pszPass) delete [] pszPass;
    if(pszNewPass)
    {
        pszPass = new char[lstrlen(pszNewPass) + 1];
        lstrcpy(pszPass, pszNewPass);
    }
    else
    {
        pszPass = NULL;
    }
    return true;
}
    
bool Katalog::PobierzHaslo(char * pszHaslo, int size)
{
    if(!pszPass)
    {
        lstrcpyn(pszHaslo, "", size);
        
        SetLastError(_ERROR_CANCEL);
        return false;
    }
    
    lstrcpyn(pszHaslo, pszPass, size);
    
    return true;
}    
