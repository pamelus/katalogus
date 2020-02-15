class Jezyk
{
    private:
        static char pszFileName[1024];
    public:
        static const int iContextBegin;
        
        static const int iMaxMenu;
        static const int iMaxDialog;
        static const int iMaxColumn;
        static const int iMaxError;
        static const int iMaxInne;
        static const int iMaxAutor;
        
        static char * pszAccel[52];
        static char * pszMenu[52];
        static char * pszDialog[134];
        static char * pszColumn[5];
        static char * pszError[5];
        static char * pszInne[16];
        static char * pszAutor[2];
        
        
        static bool Inicjuj(const char * pszFile);
        static bool Zamknij();
        
        static bool Domyslne(HINSTANCE hThis);
        static bool GetFileName(char * pszFile, int iSize) { if(pszFileName) lstrcpyn(pszFile, pszFileName, iSize); else lstrcpyn(pszFile, "", iSize); }
        static bool IsValidFileName(const char * pszName) { return (lstrcmp(pszName, pszFileName) == 0) ? true : false; }
};

const int Jezyk::iContextBegin = 37;

const int Jezyk::iMaxDialog = 134;
const int Jezyk::iMaxMenu = 52;
const int Jezyk::iMaxColumn = 5;
const int Jezyk::iMaxError = 5;
const int Jezyk::iMaxInne = 16;
const int Jezyk::iMaxAutor = 2;

char * Jezyk::pszAccel[52];
char * Jezyk::pszMenu[52];
char * Jezyk::pszDialog[134];
char * Jezyk::pszColumn[5];
char * Jezyk::pszError[5];
char * Jezyk::pszInne[16];
char * Jezyk::pszAutor[2];

char Jezyk::pszFileName[1024] = "";
