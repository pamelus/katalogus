DWORD WINAPI SaveThread(LPVOID lParam)
{
    DBFILETAG * dbf = (DBFILETAG *)lParam;
    
    FILE * pFile = fopen(dbf->pszFile, "wb");
    if(!pFile)
    {
        switch(GetLastError())
        {
            case ERROR_ACCESS_DENIED:
            {
                dbf->error = _ERROR_FILEACCESSDENIED;
                break;
            }
            default:
            {
                dbf->error = _ERROR_FILENOTCREATED;
                break;
            }
        }
        
        dbf->result = false;
        dbf->koniec = true;
        return 0;
    }
    
    BazaDanych * db = dbf->katalog->db;
    db = (db->ZamknijPozycje())->ZamknijPozycje();
    
    DWORD katID = 0x0DFA1B;
    fwrite(&katID, sizeof(DWORD), 1, pFile);
    
    char pszPass[256]; dbf->katalog->PobierzHaslo(pszPass, 256);
    for(int i = 0; i < lstrlen(pszPass); i++)
            pszPass[i] += 113;
    fwrite(pszPass, 256 * sizeof(char), 1, pFile);
    
    int iAutor = db->PobierzLiczbePozycji();
    fwrite(&iAutor, sizeof(int), 1, pFile);
    
    SendMessage(dbf->hProgress, PBM_SETRANGE32, 0, iAutor);
    
    for(int i = 0; i < iAutor; i++)
    {
        DANE dane; db->PobierzDane(i, &dane);
        fwrite(&dane, sizeof(DANE), 1, pFile);
    }
    
    for(int i = 0; i < iAutor; i++)
    {
        db = db->OtworzPozycje(i);
        
        int iAlbum = db->PobierzLiczbePozycji();
        fwrite(&iAlbum, sizeof(int), 1, pFile);
        
        for(int j = 0; j < iAlbum; j++)
        {
            DANE dane; db->PobierzDane(j, &dane);
            fwrite(&dane, sizeof(DANE), 1, pFile);
        }
        
        for(int j = 0; j < iAlbum; j++)
        {
            db = db->OtworzPozycje(j);
            
            int iAudio = db->PobierzLiczbePozycji();
            fwrite(&iAudio, sizeof(int), 1, pFile);
            
            for(int k = 0; k < iAudio; k++)
            {
                DANE dane; db->PobierzDane(k, &dane);
                fwrite(&dane, sizeof(DANE), 1, pFile);
            }
            
            db = db->ZamknijPozycje();
        }
        
        db = db->ZamknijPozycje();
        
        SendMessage(dbf->hProgress, PBM_STEPIT, 0, 0);
    }    
    
    fclose(pFile);
    
    dbf->error = 0;
    dbf->result = true;
    dbf->koniec = true;
    return 0;
}
