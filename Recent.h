class Recent
{
    private:
        static char * pszFiles[5];
    public:
        static bool Inicjuj();
        static bool Zamknij();
        
        static bool DodajPlik(const char * pszFile);
        static bool UsunPlik(const char * pszFile);
        
        static bool PobierzPlik(int iItem, char * pszFile, int iSize);
        
        static bool UaktualnijMenu(HMENU hMenu);
};

char * Recent::pszFiles[5];
