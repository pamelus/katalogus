class Rekord
{
    private:
        BazaDanych * katalog;
        BazaDanych * rodzic;
        
        DANE dane;
    public:
        Rekord(BazaDanych * nRodzic);
        ~Rekord();
                        
        BazaDanych * PobierzKatalog() { return katalog; }
        BazaDanych * PobierzRodzica() { return rodzic; }
        
        bool UstawDane(const DANE * pDane);
        bool PobierzDane(DANE * pDane);
};
