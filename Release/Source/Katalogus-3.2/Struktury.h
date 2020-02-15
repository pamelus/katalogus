// Predefinicja bazy danych
class BazaDanych; class Katalog;

// struktura przechowuj¹ca dane o ka¿dym elemencie
typedef struct
{
    char nazwa[256];
    char nosnik[256];
    char opis[256];
} DANE;


// struktura wykorzystywana przy wczytywaniu i zapisywaniu plików
typedef struct
{
    HWND hProgress;
    char * pszFile;
    Katalog * katalog;
    
    bool koniec;
    bool result;
    
    DWORD error;
} DBFILETAG;

// struktura wykorzystywana przy sortowaniu
typedef struct
{
    HWND hProgress;
    BazaDanych * db;
    WORD code;
    
    bool koniec;
    bool result;
    
    DWORD error;
} SORTTAG;
 
// struktura wykorzystywana przy skanowaniu dysku
typedef struct
{
    BazaDanych * db;
    FILE * pExt;
    
    HWND hWidok;
    HWND hDialog;
    int iImage;
    
    bool autor;
    bool album;
    bool audio;
    
    bool yes;
    bool no;
    bool koniec;
        
    DANE dane;
} SCANTAG;

typedef struct
{
    SCANTAG scn;
    HDROP hDrop;
    char * pszFolder;
} SCANINITTAG;

// struktura zawieraj¹ca dane o wynikach wyszukiwania
typedef struct
{
    char pszNazwa[256];
    char pszNosnik[256];
    char pszSciezka[1024];
    
    int iImage;    
} RESULTTAG;

// struktura zawieraj¹ca dane o sposobie wyszukiwania
typedef struct
{
    RESULTTAG * rslt;
    int iRes;
    
    HWND hProgress;
    
    char pszFraza[256];
    bool autor;
    bool album;
    bool audio;
    
    int poziom;
} FINDTAG;

typedef struct
{
    BazaDanych * db;
    WORD code;
 
    FINDTAG fnd;
    
    bool koniec;
} SEARCHTAG;    

// struktura zawieraj¹ca statystykê bazy danych
typedef struct
{
    int iAutor;
    int iAlbum;
    int iAudio;
    
    char pszDatabase[256];
} STATSTAG;

// struktura wykorzystywana przy tworzeniu raportu
typedef struct
{   
    char pszFile[1024];
    char pszTitle[256];
    char pszStop[2][16];
    char pszHeaders[3][256];
    
    LOGFONT fonts[6];
    COLORREF colors[6];
    
    int dane[3];
    
    bool kaskada[2];
    bool options[4];
    
} REPORTTAG;

typedef struct
{
    BazaDanych * db;
    REPORTTAG rprt;
    
    HWND hProgress;
    
    bool koniec;
    bool result;
    
    DWORD error;
} REPORTINITTAG;    





