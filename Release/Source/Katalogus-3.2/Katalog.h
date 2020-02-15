class Rekord;

#include "Struktury.h"
#include "BazaDanych.h"
#include "Rekord.h"

class Katalog
{
    private:
        char * pszFile;
        char * pszPass;
        
        bool modified;
        int poziom;
        const int iMaxPoziom;
    public:
        // Konstruktor i destruktor
        Katalog();
        ~Katalog();
        
        // Og�lnodost�pna baza danych. Dzi�ki temu nie trzeba w klasie Katalog
        // implementowa� wszystkich funkcji bazy dasnych.
        BazaDanych * db;
        
        // Zwraca aktualny poziom bazy danych. Poziom jest zwi�szany przy przej�ciu
        // "g��biej" a zmniejszany przy przejs�iu wy�ej.
        int PobierzPoziom() { return poziom; }
        
        // Funckje przechodz�ce do podrz�dnych i nadrz�dnych baz danych.
        bool Otworz(int index);
        bool Wstecz();
        
        // Funckje charakteryzuj�e stan bazy danych (zpisana lub nie)
        bool Modyfikacja() { return modified; }
        bool UstawModyfikacja(bool mod) { modified = mod; }
        
        // Ustawia plik bazy danych, lub kasuje go (je�eli NULL)
        bool UstawPlik(const char * pszNewFile);
        
        // Pobiera plik bazy danych lub kopiuje "" i zwraca false, je�eli nie ma pliku
        bool PobierzPlik(char * pszPlik, int size);
        
        // Ustawia i pobiera has�o. Zwraca false, je�eli nie ma has�a.
        bool UstawHaslo(const char * pszNewPass);
        bool PobierzHaslo(char * pszHaslo, int size);
};

#include "BazaDanych.hpp"
#include "Rekord.hpp"
