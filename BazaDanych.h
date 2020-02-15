class BazaDanych
{
    private:
        Rekord ** rekord;
        Rekord * rodzic;
        int liczba;
        int otwarty;
    public:
        // Konstruktor i destruktor
        BazaDanych(Rekord * nRodzic);
        ~BazaDanych();
        
        // Funckje operuj�ce na bazie danych. Nazwy m�wi� same sa siebie
        bool DodajPozycja(const DANE * dane);
        bool EdytujPozycja(int index, const DANE * dane);
        bool UsunPozycja(int index);
        
        bool PrzeniesWGore(int index);
        bool PrzeniesWDol(int index);
        
        // Funkcje szereguj�ce elementy w bazie danych
        bool SzeregujNazwa(HWND hProgress);
        bool SzeregujNosnik(HWND hProgress);
        bool SzeregujOpis(HWND hProgress);
        
        // Funckje dost�pu do danych
        bool PobierzDane(int index, DANE * dane);
        bool PobierzSciezke(char * pszPath, int size);
        int PobierzLiczbePozycji() { return liczba; }
        
        // Funckje dost�pu do podrz�dnych i nadrz�dnych baz danych
        BazaDanych * OtworzPozycje(int index);
        BazaDanych * ZamknijPozycje();
        
        // Funckje wyszukuj�ce. Zwracaj� liczb� element�w wyszukanych znajduj�cych si�
        // w utworzonej przez funkcj� tablicy RESULTTAG znajduj�cej si� w strukturze
        // FINDTAG
        int SzukajNazwa(FINDTAG * fnd);
        int SzukajNosnik(FINDTAG * fnd);
        
        // Sprawdza, czy pozycja o danej nazwie nie istnieje w bazie danych, a je�eli tak,
        // to zwraca jej index.
        int SprawdzPozycja(const char * pszNazwa);
        
        // Ustawia i pobiera aktualnie otwarty element w bazie danych. Dzi�i temu, mo�na ustawi�
        // zaznaczenie przy Wstecz
        void UstawOtwarty(int iValue) { otwarty = iValue; }
        int PobierzOtwarty() { return otwarty; }
};
