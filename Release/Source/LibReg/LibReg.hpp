RKey::RKey()
{
    hKey = NULL;
}

RKey::~RKey()
{
    if(hKey) this->Zamknij();
}

bool RKey::Otworz(HKEY hParent, const char * pszKey, REGSAM dSam)
{
    if(hKey) this->Zamknij();
    if(RegOpenKeyEx(hParent, pszKey, 0, dSam, &hKey) != ERROR_SUCCESS) return false;
    return true;
}

bool RKey::Zamknij()
{
    if(hKey) RegCloseKey(hKey);
    hKey = NULL;
    return true;
}

bool RKey::ZapiszRekordA(const char * pszName, bool bValue)
{
    DWORD dValue = 0;
    if(bValue) dValue = 1;
    return this->ZapiszRekordB(pszName, dValue);
}

bool RKey::ZapiszRekordB(const char * pszName, DWORD dValue)
{
    if(RegSetValueEx(hKey, pszName, 0, REG_DWORD, (CONST BYTE*)&dValue, sizeof(DWORD)) != ERROR_SUCCESS) return false;
    return true;
}

bool RKey::ZapiszRekordC(const char * pszName, const char * pszValue)
{
    if(RegSetValueEx(hKey, pszName, 0, REG_SZ, (CONST BYTE*)pszValue, lstrlen(pszValue) + 1) != ERROR_SUCCESS) return false;
    return true;
}

bool RKey::ZapiszRekordD(const char * pszName, LPVOID binValue, DWORD dSize)
{
    if(RegSetValueEx(hKey, pszName, 0, REG_BINARY, (CONST BYTE*)binValue, dSize) != ERROR_SUCCESS) return false;
    return true;
}

bool RKey::WczytajRekordA(const char * pszName, bool & rbValue)
{
    DWORD dValue = 0;
    
    if(!this->WczytajRekordB(pszName, dValue)) return false;
    
    rbValue = true;
    if(dValue == 0) rbValue = false;
    return true;
}

bool RKey::WczytajRekordB(const char * pszName, DWORD & rdValue)
{
    DWORD size = 0;
    if(RegQueryValueEx(hKey, pszName, 0, NULL, NULL, &size) != ERROR_SUCCESS) return false;
    if(RegQueryValueEx(hKey, pszName, 0, NULL, (BYTE*)&rdValue, &size) != ERROR_SUCCESS) return false;
    return true;
}

bool RKey::WczytajRekordC(const char * pszName, char * pszValue, DWORD dSize)
{
    DWORD size = 0;
    if(RegQueryValueEx(hKey, pszName, 0, NULL, NULL, &size) != ERROR_SUCCESS) return false;
    
    char pszBufer[size];
    if(RegQueryValueEx(hKey, pszName, 0, NULL, (BYTE*)pszBufer, &size) != ERROR_SUCCESS) return false;
    
    if(pszValue) lstrcpyn(pszValue, pszBufer, dSize);
    return true;
}

bool RKey::WczytajRekordD(const char * pszName, LPVOID binValue, DWORD dSize)
{
    if(RegQueryValueEx(hKey, pszName, 0, NULL, (BYTE*)binValue, &dSize) != ERROR_SUCCESS) return false;
    return true;
}

DWORD RKey::PobierzRozmiar(const char * pszName)
{
    DWORD size = 0;
    if(RegQueryValueEx(hKey, pszName, 0, NULL, NULL, &size) != ERROR_SUCCESS) return 0;
    
    return size;
}
