BazaDanych::BazaDanych(Rekord * nRodzic):
rodzic(nRodzic), otwarty(0)
{
    rekord = NULL;
    liczba = 0;
}

BazaDanych::~BazaDanych()
{
    for(int i = 0; i < liczba; i++)
        delete rekord[i];
    
    if(rekord) delete [] rekord;
    liczba = 0;
}

bool BazaDanych::DodajPozycja(const DANE * dane)
{
    Rekord ** temp = new (Rekord *)[liczba + 1];
    temp[liczba] = new Rekord(this);
    temp[liczba]->UstawDane(dane);
    
    for(int i = 0; i < liczba; i++)
        temp[i] = rekord[i];
    
    delete [] rekord;
    rekord = temp;
    
    liczba++;
    
    return true;
}

bool BazaDanych::EdytujPozycja(int index, const DANE * dane)
{
    if(index < 0 || index >= liczba)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    rekord[index]->UstawDane(dane);
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

bool BazaDanych::UsunPozycja(int index)
{
    if(index < 0 || index >= liczba)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    delete rekord[index];
    rekord[index] = NULL;
    
    Rekord ** temp = new (Rekord *)[liczba - 1];
    
    for(int i = 0, j = 0; i < liczba; i++)
    {
        if(i != index)
        {
            temp[j] = rekord[i];
            j++;
        }
    }
    
    delete [] rekord;
    rekord = temp;
    
    liczba--;
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

bool BazaDanych::PrzeniesWGore(int index)
{
    if(index < 1 || index >= liczba)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    Rekord * temp = rekord[index - 1];
    rekord[index - 1] = rekord[index];
    rekord[index] = temp;
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

bool BazaDanych::PrzeniesWDol(int index)
{
    if(index < 0 || index >= liczba - 1)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }
    
    Rekord * temp = rekord[index + 1];
    rekord[index + 1] = rekord[index];
    rekord[index] = temp;
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

// Do sortowania u¿yto algorytmu Shella 
// (zwanego te¿ sortowaniem przy malej¹cych przyrostach)
// Jest on conajmniej 1000 razy szybszy od b¹belkowego (przy du¿ych tablicach)!!!

bool BazaDanych::SzeregujNazwa(HWND hProgress)
{
    int h = 1;
    int range = 1;
    do
    {
        h = 3 * h + 1;
        range++;
    }
    while(h <= liczba / 9);
    
    SendMessage(hProgress, PBM_SETRANGE32, 0, range);
    
    DANE dane1, dane2;
    
    while(h >= 1)
    {
        for(int i = h; i < liczba; i++)
        {
            rekord[i]->PobierzDane(&dane1);
            rekord[i - h]->PobierzDane(&dane2);
            
            if(lstrcmp(dane1.nazwa, dane2.nazwa) < 0)
            {
                Rekord * temp = rekord[i];
                rekord[i] = rekord[i - h];
                rekord[i - h] = temp;
                
                for(int j = i - h; j >= h; j-=h)
                {
                    rekord[j]->PobierzDane(&dane1);
                    rekord[j - h]->PobierzDane(&dane2);
                    
                    if(lstrcmp(dane1.nazwa, dane2.nazwa) < 0)
                    {
                        Rekord * temp = rekord[j];
                        rekord[j] = rekord[j - h];
                        rekord[j - h] = temp;
                    }
                    else
                    {
                        break;
                    }
                }
            }    
        }
        
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        h = (h - 1) / 3;
    }    
    return true;
}
    
bool BazaDanych::SzeregujNosnik(HWND hProgress)
{
    int h = 1;
    int range = 1;
    do
    {
        h = 3 * h + 1;
        range++;
    }
    while(h <= liczba / 9);
    
    SendMessage(hProgress, PBM_SETRANGE32, 0, range);
    
    DANE dane1, dane2;
    
    while(h >= 1)
    {
        for(int i = h; i < liczba; i++)
        {
            rekord[i]->PobierzDane(&dane1);
            rekord[i - h]->PobierzDane(&dane2);
            
            if(lstrcmp(dane1.nosnik, dane2.nosnik) < 0)
            {
                Rekord * temp = rekord[i];
                rekord[i] = rekord[i - h];
                rekord[i - h] = temp;
                
                for(int j = i - h; j >= h; j-=h)
                {
                    rekord[j]->PobierzDane(&dane1);
                    rekord[j - h]->PobierzDane(&dane2);
                    
                    if(lstrcmp(dane1.nosnik, dane2.nosnik) < 0)
                    {
                        Rekord * temp = rekord[j];
                        rekord[j] = rekord[j - h];
                        rekord[j - h] = temp;
                    }
                    else
                    {
                        break;
                    }
                }
            }    
        }
        
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        h = (h - 1) / 3;
    }    
    
    return true;
}
    
bool BazaDanych::SzeregujOpis(HWND hProgress)
{
    int h = 1;
    int range = 1;
    do
    {
        h = 3 * h + 1;
        range++;
    }
    while(h <= liczba / 9);
    
    SendMessage(hProgress, PBM_SETRANGE32, 0, range);
    
    DANE dane1, dane2;
    
    while(h >= 1)
    {
        for(int i = h; i < liczba; i++)
        {
            rekord[i]->PobierzDane(&dane1);
            rekord[i - h]->PobierzDane(&dane2);
            
            if(lstrlen(dane1.opis) > lstrlen(dane2.opis))
            {
                Rekord * temp = rekord[i];
                rekord[i] = rekord[i - h];
                rekord[i - h] = temp;
                
                for(int j = i - h; j >= h; j-=h)
                {
                    rekord[j]->PobierzDane(&dane1);
                    rekord[j - h]->PobierzDane(&dane2);
                    
                    if(lstrlen(dane1.opis) > lstrlen(dane2.opis))
                    {
                        Rekord * temp = rekord[j];
                        rekord[j] = rekord[j - h];
                        rekord[j - h] = temp;
                    }
                    else
                    {
                        break;
                    }
                }
            }    
        }
        
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        h = (h - 1) / 3;
    }    
    
    return true;
}    

bool BazaDanych::PobierzDane(int index, DANE * dane)
{
    if(index < 0 || index >= liczba)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return false;
    }    
    
    rekord[index]->PobierzDane(dane);
    
    SetLastError(_ERROR_SUCCESS);
    return true;
}

bool BazaDanych::PobierzSciezke(char * pszPath, int size)
{
    if(rodzic)
    {
        (rodzic->PobierzRodzica())->PobierzSciezke(pszPath, size);
        
        DANE dane; rodzic->PobierzDane(&dane);
        int lenght = 0;
        lenght += lstrlen(pszPath) + 1;
        lenght += lstrlen(dane.nazwa) + 1;
        
        char pszTemp[lenght];
        lstrcpy(pszTemp, pszPath);
        if(lstrlen(pszTemp) > 0) lstrcat(pszTemp, " - ");
        lstrcat(pszTemp, dane.nazwa);
        
        lstrcpyn(pszPath, pszTemp, size); 
    }
    else
    {
        lstrcpyn(pszPath, "", size);
    }
    
    return true;
}

BazaDanych * BazaDanych::OtworzPozycje(int index)
{
    if(index < 0 || index >= liczba)
    {
        SetLastError(_ERROR_OVERDRIVE);
        return this;
    }
    
    SetLastError(_ERROR_SUCCESS);
    return rekord[index]->PobierzKatalog();
}
    
BazaDanych * BazaDanych::ZamknijPozycje()
{
    if(rodzic)
    {
        SetLastError(_ERROR_SUCCESS);
        return rodzic->PobierzRodzica();
    }
    else
    {
        SetLastError(_ERROR_OVERDRIVE);
        return this;
    }
}

int BazaDanych::SzukajNazwa(FINDTAG * fnd)
{    
    if((fnd->poziom == 0 && fnd->autor) || (fnd->poziom == 1 && fnd->album) || (fnd->poziom == 2 && fnd->audio))
    {        
        for(int i = 0; i < liczba; i++)
        {
            DANE dane;
            rekord[i]->PobierzDane(&dane);
            
            char pszNazwa1[256]; lstrcpy(pszNazwa1, dane.nazwa);
            char pszNazwa2[256]; lstrcpy(pszNazwa2, fnd->pszFraza);
            
            CharLower(pszNazwa1);
            CharLower(pszNazwa2);
            
            int iSize = lstrlen(pszNazwa2);
            
            if(strncmp(pszNazwa2, pszNazwa1, iSize) == 0)
            {
                RESULTTAG * temp = new RESULTTAG[fnd->iRes + 1];
                if(fnd->rslt)
                {
                    CopyMemory(temp, fnd->rslt, fnd->iRes * sizeof(RESULTTAG));
                    delete [] fnd->rslt;
                }
                
                fnd->rslt = temp;
                
                lstrcpy(fnd->rslt[fnd->iRes].pszNazwa, dane.nazwa);
                lstrcpy(fnd->rslt[fnd->iRes].pszNosnik, dane.nosnik);
                this->PobierzSciezke(fnd->rslt[fnd->iRes].pszSciezka, 1024);
                
                fnd->rslt[fnd->iRes].iImage = fnd->poziom;
                                
                fnd->iRes++;
            }
        }
    }
    
    if(fnd->poziom < 2)
    {
        HWND hProgress = fnd->hProgress;
        fnd->hProgress = NULL;
        
        SendMessage(hProgress, PBM_SETRANGE32, 0, liczba);
        
        for(int i = 0; i < liczba; i++)
        {
            fnd->poziom++;
            
            (rekord[i]->PobierzKatalog())->SzukajNazwa(fnd);
            
            fnd->poziom--;
            
            SendMessage(hProgress, PBM_STEPIT, 0, 0);
        }
        fnd->hProgress = hProgress;
    }
    
    return fnd->iRes;
}

int BazaDanych::SzukajNosnik(FINDTAG * fnd)
{
    if((fnd->poziom == 0 && fnd->autor) || (fnd->poziom == 1 && fnd->album) || (fnd->poziom == 2 && fnd->audio))
    {       
        for(int i = 0; i < liczba; i++)
        {
            DANE dane;
            rekord[i]->PobierzDane(&dane);
            
            char pszNazwa1[256]; lstrcpy(pszNazwa1, dane.nosnik);
            char pszNazwa2[256]; lstrcpy(pszNazwa2, fnd->pszFraza);
            
            CharLower(pszNazwa1);
            CharLower(pszNazwa2);
            
            if(lstrcmp(pszNazwa2, pszNazwa1) == 0)
            {
                RESULTTAG * temp = new RESULTTAG[fnd->iRes + 1];
                if(fnd->rslt)
                {
                    CopyMemory(temp, fnd->rslt, fnd->iRes * sizeof(RESULTTAG));
                    delete [] fnd->rslt;
                }
                
                fnd->rslt = temp;
                
                lstrcpy(fnd->rslt[fnd->iRes].pszNazwa, dane.nazwa);
                lstrcpy(fnd->rslt[fnd->iRes].pszNosnik, dane.nosnik);
                this->PobierzSciezke(fnd->rslt[fnd->iRes].pszSciezka, 1024);
                
                fnd->rslt[fnd->iRes].iImage = fnd->poziom;
                
                fnd->iRes++;
            }
        }
    }
    
    if(fnd->poziom < 2)
    {
        HWND hProgress = fnd->hProgress;
        fnd->hProgress = NULL;
        
        SendMessage(hProgress, PBM_SETRANGE32, 0, liczba);
        
        for(int i = 0; i < liczba; i++)
        {
            fnd->poziom++;
            
            (rekord[i]->PobierzKatalog())->SzukajNosnik(fnd);
            
            fnd->poziom--;
            
            SendMessage(hProgress, PBM_STEPIT, 0, 0);
        }
        fnd->hProgress = hProgress;
    }
    
    return fnd->iRes;
}

int BazaDanych::SprawdzPozycja(const char * pszNazwa)
{
    for(int i = 0; i < liczba; i++)
    {
        DANE dane; rekord[i]->PobierzDane(&dane);
        
        char pszNazwa1Low[256]; lstrcpy(pszNazwa1Low, pszNazwa);
        char pszNazwa2Low[256]; lstrcpy(pszNazwa2Low, dane.nazwa);
        
        CharLower(pszNazwa1Low);
        CharLower(pszNazwa2Low);
        
        if(lstrcmp(pszNazwa1Low, pszNazwa2Low) == 0) return i;
    }
    return -1;
}
