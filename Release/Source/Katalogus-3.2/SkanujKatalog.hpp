bool SkanujKatalog(char * pszFile, SCANTAG * scn)
{
    if(scn->koniec) return true;
    if(PathIsDirectory(pszFile))
    {
        SetCurrentDirectory(pszFile);
             
        WIN32_FIND_DATA wfd; ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
        HANDLE hFile = FindFirstFile("*.*", &wfd);
                
        do
        {
            if(wfd.cFileName[0] == '.') continue;
            
            char pszNewFile[1024];
            GetFullPathName(wfd.cFileName, 1024, pszNewFile, NULL);
            SkanujKatalog(pszNewFile, scn);
            if(scn->koniec) break;      
        }
        while(FindNextFile(hFile, &wfd));
        FindClose(hFile);
                
        SetCurrentDirectory("..");
    }
    else
    {
        SendMessage(scn->hDialog, WM_SETPATH, 0, (LPARAM)pszFile);
        if(scn->pExt)
        {
            rewind(scn->pExt);
            char pszExt1[16]; char pszExt2[1024];
            lstrcpyn(pszExt1, PathFindExtension(pszFile), 16);
            fgets(pszExt2, 1024, scn->pExt);
            
            bool isValidExt = false;
            
            for(int i = 0, iLast = 0; i < lstrlen(pszExt2); i++)
            {
                if(pszExt2[i] == ';')
                {
                    if(strncmp(pszExt1, &pszExt2[iLast], i - iLast) == 0)
                    {
                        isValidExt = true;
                        break;
                    }    
                    iLast = i + 1;
                }
            }
            
            if(!isValidExt) return true;
        }    
        
        DANE dane[3] = {  { "", "", "" }, { "", "", "" }, { "", "", "" } };
        if(scn->autor) CopyMemory(&dane[0], &scn->dane, sizeof(DANE));
        if(scn->album) CopyMemory(&dane[1], &scn->dane, sizeof(DANE));
        if(scn->audio) CopyMemory(&dane[2], &scn->dane, sizeof(DANE));
        
        ID3GetAutor(pszFile, dane[0].nazwa, 256);
        ID3GetAlbum(pszFile, dane[1].nazwa, 256);
        ID3GetTitle(pszFile, dane[2].nazwa, 256);
        
        for(int i = 0; i < 3; i++)
        {
            if(dane[i].nazwa[0] == 0)
            {
                if((i == 0 && Opcje::IsFolderAutor()) || (i == 1 && Opcje::IsFolderAlbum()) || (i == 2 && Opcje::IsFolderAudio()))
                {    
                    char pszName[1024]; lstrcpy(pszName, pszFile);
                    PathRemoveExtension(pszName);
                    for(int j = i; j < 2; j++)
                        PathRemoveFileSpec(pszName);
                    
                    lstrcpyn(dane[i].nazwa, PathFindFileName(pszName), 256);
                }
            }    
        }
        
        if(dane[2].nazwa[0] == 0) return true;
        if(dane[1].nazwa[0] == 0) lstrcpy(dane[1].nazwa, "???");
        if(dane[0].nazwa[0] == 0) lstrcpy(dane[0].nazwa, "???");
        
        int iItem[3] = {-1, -1, -1};
        
        BazaDanych * db = scn->db;
        
        for(int i = scn->iImage; i < 3; i++)
        {
            iItem[i] = db->SprawdzPozycja(dane[i].nazwa);
            if(iItem[i] < 0)
            {
                iItem[i] = db->PobierzLiczbePozycji();
                db->DodajPozycja(&dane[i]);
                if(i == scn->iImage)
                    ListView_AddItems(scn->hWidok, i, &dane[i], 1);
            }
            else if (i == 2)
            {
                if(!scn->yes && !scn->no)
                {
                    char pszPath[1024]; db->PobierzSciezke(pszPath, 1024);
                
                    char pszMessage[2048];
                    wsprintf(pszMessage, Jezyk::pszDialog[131], pszPath, dane[i].nazwa);
                
                    int iCode = DialogBoxParam(GetModuleHandle(NULL), "MESSAGE_DIALOG", scn->hDialog, MessageProc, (LPARAM)pszMessage);
                    switch(iCode)
                    {
                        case IDCBOX_YESTOALL:
                            scn->yes = true;
                        case IDCBOX_YES:
                            db->DodajPozycja(&dane[i]);
                            if(i == scn->iImage)
                                ListView_AddItems(scn->hWidok, scn->iImage, &dane[i], 1);
                            break;
                        case IDCBOX_NOTOALL:
                            scn->no = true;
                        case IDCBOX_NO:
                            break;
                    }
                    SetFocus(scn->hDialog);
                }
                else if (scn->yes)
                {
                    db->DodajPozycja(&dane[i]);
                    if(i == scn->iImage)
                        ListView_AddItems(scn->hWidok, scn->iImage, &dane[i], 1);
                }
            }
            if(i < 2)
            {
                db = db->OtworzPozycje(iItem[i]);
            }
        }
    }
    return true;
}
