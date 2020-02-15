DWORD WINAPI LoadThread(LPVOID lParam)
{
    DBFILETAG * dbf = (DBFILETAG *)lParam;
    
    FILE * pFile = fopen(dbf->pszFile, "rb");
    
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
                dbf->error = _ERROR_FILENOTOPENED;
                break;
            }
        }
        dbf->result = false;
        dbf->koniec = true;
        return 0;
    }    
    
    BazaDanych * db = dbf->katalog->db;
    db = (db->ZamknijPozycje())->ZamknijPozycje();
    
    DWORD katID;
    fread(&katID, sizeof(DWORD), 1, pFile);
    if(katID != 0x0DFA1B)
    {
        dbf->error = _ERROR_NOTDATABASE;
        dbf->result = false;
        dbf->koniec = true;
        return 0;
    }
    
    char pszPass[256];
    fread(pszPass, 256 * sizeof(char), 1, pFile);
    
    if(pszPass[0] != 0)
    {
        for(int i = 0; i < lstrlen(pszPass); i++)
            pszPass[i] -= 113;
            
        char pszParam[2][1024];
        lstrcpyn(pszParam[0], pszPass, 1024);
        lstrcpyn(pszParam[1], dbf->pszFile, 1024);
        
        int iCode = DialogBoxParam(GetModuleHandle(NULL), "PSSWD_DIALOG", hOkna, PsswdProc, (LPARAM)pszParam);
        if(iCode != IDCBOX_OK)
        {
            dbf->error = _ERROR_CANCEL;
            dbf->result = false;
            dbf->koniec = true;
            return 0;
        }
        
        dbf->katalog->UstawHaslo(pszPass);
    }
    
    int iAutor;
    fread(&iAutor, sizeof(int), 1, pFile);
    
    SendMessage(dbf->hProgress, PBM_SETRANGE32, 0, iAutor);
    
    DANE dane1[iAutor];
    fread(dane1, sizeof(DANE), iAutor, pFile);
    
    for(int i = 0; i < iAutor; i++)
        db->DodajPozycja(&dane1[i]);
    for(int i = 0; i < iAutor; i++)
    {
        db = db->OtworzPozycje(i);
        
        int iAlbum;
        fread(&iAlbum, sizeof(int), 1, pFile);
        
        DANE dane2[iAlbum];
        fread(dane2, sizeof(DANE), iAlbum, pFile);
        
        for(int j = 0; j < iAlbum; j++)
            db->DodajPozycja(&dane2[j]);
        for(int j = 0; j < iAlbum; j++)
        {
            db = db->OtworzPozycje(j);
            
            int iAudio;
            fread(&iAudio, sizeof(int), 1, pFile);
            
            DANE dane3[iAudio];
            fread(dane3, sizeof(DANE), iAudio, pFile);
            
            for(int k = 0; k < iAudio; k++)
                db->DodajPozycja(&dane3[k]);
            
            db = db->ZamknijPozycje();
        }    
        
        db = db->ZamknijPozycje();
        SendMessage(dbf->hProgress, PBM_STEPIT, 0, 0);
    }
        
    fclose(pFile);
    
    dbf->error = _ERROR_SUCCESS;
    dbf->result = true;
    dbf->koniec = true;
    return 0;
}
