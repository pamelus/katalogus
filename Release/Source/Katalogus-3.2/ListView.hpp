int ListView_AddItems(HWND hWidok, int iImage, DANE * dane, int liczba)
{
    LV_ITEM lvi;
    lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvi.iImage = iImage;
    lvi.iSubItem = 0;
    lvi.state = 0;
    lvi.stateMask = 0;
    
    int iCount = ListView_GetItemCount(hWidok);
                    
    for(int i = 0; i < liczba; i++)
    {
        int iItem = iCount + i;
        
        lvi.iItem = iItem;
        ListView_InsertItem(hWidok, &lvi);
                        
        char pszOpis[16]; wsprintf(pszOpis, "%i %s", lstrlen(dane[i].opis), Jezyk::pszInne[2]);
        char pszIndex[16]; wsprintf(pszIndex, "%i", iItem + 1);
                        
        ListView_SetItemText(hWidok, iItem, 0, dane[i].nazwa);
        ListView_SetItemText(hWidok, iItem, 1, dane[i].nosnik);
        ListView_SetItemText(hWidok, iItem, 2, pszOpis);
        ListView_SetItemText(hWidok, iItem, 3, pszIndex);
    }
    
    return iCount + liczba - 1;
}
