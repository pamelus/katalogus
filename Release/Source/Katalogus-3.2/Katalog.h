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
        
        // Ogólnodostêpna baza danych. Dziêki temu nie trzeba w klasie Katalog
        // implementowaæ wszystkich funkcji bazy dasnych.
        BazaDanych * db;
        
        // Zwraca aktualny poziom bazy danych. Poziom jest zwiêszany przy przejœciu
        // "g³êbiej" a zmniejszany przy przejsæiu wy¿ej.
        int PobierzPoziom() { return poziom; }
        
        // Funckje przechodz¹ce do podrzêdnych i nadrzêdnych baz danych.
        bool Otworz(int index);
        bool Wstecz();
        
        // Funckje charakteryzuj¹e stan bazy danych (zpisana lub nie)
        bool Modyfikacja() { return modified; }
        bool UstawModyfikacja(bool mod) { modified = mod; }
        
        // Ustawia plik bazy danych, lub kasuje go (je¿eli NULL)
        bool UstawPlik(const char * pszNewFile);
        
        // Pobiera plik bazy danych lub kopiuje "" i zwraca false, je¿eli nie ma pliku
        bool PobierzPlik(char * pszPlik, int size);
        
        // Ustawia i pobiera has³o. Zwraca false, je¿eli nie ma has³a.
        bool UstawHaslo(const char * pszNewPass);
        bool PobierzHaslo(char * pszHaslo, int size);
};

#include "BazaDanych.hpp"
#include "Rekord.hpp"
