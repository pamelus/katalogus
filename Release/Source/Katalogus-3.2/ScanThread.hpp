DWORD WINAPI ScanThread(LPVOID lParam)
{
    SCANINITTAG * sInit = (SCANINITTAG *)lParam;
    
    if(sInit->hDrop)
    {
        int iCount = DragQueryFile(sInit->hDrop, 0xFFFFFFFF, NULL, 0);
            
        for(int i = 0; i < iCount; i++)
        {
            char pszFile[1024]; DragQueryFile(sInit->hDrop, i, pszFile, 1024);
            SkanujKatalog(pszFile, &sInit->scn);
            if(sInit->scn.koniec) break;
        }
    }
    else if(sInit->pszFolder)
    {
        SkanujKatalog(sInit->pszFolder, &sInit->scn);
    }
    
    SendMessage(sInit->scn.hDialog, WM_COMMAND, MAKELONG(IDC_KONIEC, 0), 0);
    return 0;
}
