void DodajDoRaportu(Katalog * report, const int * iDane, const DANE * dane1, const DANE * dane2, const DANE * dane3)
{
    char pszDane[3][256] = {"", "", ""};
    for(int i = 0; i < 3; i++)
    {
        switch(iDane[i])
        {
            case 1: if(dane1) lstrcpy(pszDane[i], dane1->nazwa); break;
            case 2: if(dane1) lstrcpy(pszDane[i], dane1->nosnik); break;
            case 3: if(dane2) lstrcpy(pszDane[i], dane2->nazwa); break;
            case 4: if(dane2) lstrcpy(pszDane[i], dane2->nosnik); break;
            case 5: if(dane3) lstrcpy(pszDane[i], dane3->nazwa); break;
            case 6: if(dane3) lstrcpy(pszDane[i], dane3->nosnik); break;
        }
    }
    
    for(int i = 0, iItem = -1; i < 3; i++)
    {
        if(iDane[i] != 0)
        {
            report->Otworz(iItem);
            
            iItem = report->db->SprawdzPozycja(pszDane[i]);
            if(iItem < 0)
            {
                iItem = report->db->PobierzLiczbePozycji();
                DANE dane = {"", "", ""}; lstrcpy(dane.nazwa, pszDane[i]);
                report->db->DodajPozycja(&dane);
            }
        }
    }
    
    report->Wstecz(); report->Wstecz();
}
