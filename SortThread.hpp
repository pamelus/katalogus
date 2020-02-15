DWORD WINAPI SortThread(LPVOID lParam)
{
    SORTTAG * srt = (SORTTAG *)lParam;
    
    switch(srt->code)
    {
        case IDC_SZEREGUJNAZWA:
            srt->result = srt->db->SzeregujNazwa(srt->hProgress);
            break;
        case IDC_SZEREGUJNOSNIK:
            srt->result = srt->db->SzeregujNosnik(srt->hProgress);
            break;
        case IDC_SZEREGUJOPIS:
            srt->result = srt->db->SzeregujOpis(srt->hProgress);
            break;
    }
    
    srt->error = (srt->result) ? 0 : GetLastError();
    srt->koniec = true;
    return 0;
}    
