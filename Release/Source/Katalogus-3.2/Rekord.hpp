Rekord::Rekord(BazaDanych * nRodzic):
rodzic(nRodzic)
{
    katalog = new BazaDanych(this);
    lstrcpy(dane.nazwa, "");
    lstrcpy(dane.nosnik, "");
    lstrcpy(dane.opis, "");
}

Rekord::~Rekord()
{
    delete katalog;
}

bool Rekord::UstawDane(const DANE * pDane)
{
    CopyMemory(&dane, pDane, sizeof(DANE));
    return true;
}
    
bool Rekord::PobierzDane(DANE * pDane)
{
    CopyMemory(pDane, &dane, sizeof(DANE));
    return true;
}
