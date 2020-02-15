DWORD WINAPI SearchThread(LPVOID lParam)
{
    SEARCHTAG * srch = (SEARCHTAG *)lParam;
    
    switch(srch->code)
    {
        case IDC_SZUKAJNAZWA:
            srch->db->SzukajNazwa(&srch->fnd);
            break;
        case IDC_SZUKAJNOSNIK:
            srch->db->SzukajNosnik(&srch->fnd);
            break;
    }
    
    srch->koniec = true;
    return 0;
}
