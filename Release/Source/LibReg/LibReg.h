extern "C"
{
    class  __declspec (dllexport)RKey
    {
        private:
            HKEY hKey;
        public:
            RKey();
            ~RKey();
            
            bool Otworz(HKEY hParent, const char * pszKey, REGSAM dSam);
            bool Zamknij();
        
            bool ZapiszRekordA(const char * pszName, bool bValue);
            bool ZapiszRekordB(const char * pszName, DWORD dValue);
            bool ZapiszRekordC(const char * pszName, const char * pszValue);
            bool ZapiszRekordD(const char * pszName, LPVOID binValue, DWORD dSize);
        
            bool WczytajRekordA(const char * pszName, bool & rbValue);
            bool WczytajRekordB(const char * pszName, DWORD & rdValue);
            bool WczytajRekordC(const char * pszName, char * pszValue, DWORD dSize);
            bool WczytajRekordD(const char * pszName, LPVOID binValue, DWORD dSize);
            
            DWORD RKey::PobierzRozmiar(const char * pszName);
    };
}
