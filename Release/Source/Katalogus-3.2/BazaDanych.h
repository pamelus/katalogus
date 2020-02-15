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
        
        // Funckje operuj¹ce na bazie danych. Nazwy mówi¹ same sa siebie
        bool DodajPozycja(const DANE * dane);
        bool EdytujPozycja(int index, const DANE * dane);
        bool UsunPozycja(int index);
        
        bool PrzeniesWGore(int index);
        bool PrzeniesWDol(int index);
        
        // Funkcje szereguj¹ce elementy w bazie danych
        bool SzeregujNazwa(HWND hProgress);
        bool SzeregujNosnik(HWND hProgress);
        bool SzeregujOpis(HWND hProgress);
        
        // Funckje dostêpu do danych
        bool PobierzDane(int index, DANE * dane);
        bool PobierzSciezke(char * pszPath, int size);
        int PobierzLiczbePozycji() { return liczba; }
        
        // Funckje dostêpu do podrzêdnych i nadrzêdnych baz danych
        BazaDanych * OtworzPozycje(int index);
        BazaDanych * ZamknijPozycje();
        
        // Funckje wyszukuj¹ce. Zwracaj¹ liczbê elementów wyszukanych znajduj¹cych siê
        // w utworzonej przez funkcjê tablicy RESULTTAG znajduj¹cej siê w strukturze
        // FINDTAG
        int SzukajNazwa(FINDTAG * fnd);
        int SzukajNosnik(FINDTAG * fnd);
        
        // Sprawdza, czy pozycja o danej nazwie nie istnieje w bazie danych, a je¿eli tak,
        // to zwraca jej index.
        int SprawdzPozycja(const char * pszNazwa);
        
        // Ustawia i pobiera aktualnie otwarty element w bazie danych. Dziêi temu, mo¿na ustawiæ
        // zaznaczenie przy Wstecz
        void UstawOtwarty(int iValue) { otwarty = iValue; }
        int PobierzOtwarty() { return otwarty; }
};
