DWORD WINAPI ReportThread(LPVOID lParam)
{
    REPORTINITTAG * rpInit = (REPORTINITTAG *)lParam;
    
    char pszStyle1[256] = "normal"; if(rpInit->rprt.fonts[0].lfItalic) lstrcpy(pszStyle1, "italic");
    char pszStyle2[256] = "normal"; if(rpInit->rprt.fonts[1].lfItalic) lstrcpy(pszStyle2, "italic");
    char pszStyle3[256] = "normal"; if(rpInit->rprt.fonts[2].lfItalic) lstrcpy(pszStyle3, "italic");
    char pszStyle4[256] = "normal"; if(rpInit->rprt.fonts[3].lfItalic) lstrcpy(pszStyle4, "italic");
    char pszStyle5[256] = "normal"; if(rpInit->rprt.fonts[4].lfItalic) lstrcpy(pszStyle5, "italic");
    char pszStyle6[256] = "normal"; if(rpInit->rprt.fonts[5].lfItalic) lstrcpy(pszStyle6, "italic");
    
    char pszDecoration1[256] = "none"; if(rpInit->rprt.fonts[0].lfUnderline) lstrcpy(pszDecoration1, "underline");
    char pszDecoration2[256] = "none"; if(rpInit->rprt.fonts[1].lfUnderline) lstrcpy(pszDecoration2, "underline");
    char pszDecoration3[256] = "none"; if(rpInit->rprt.fonts[2].lfUnderline) lstrcpy(pszDecoration3, "underline");
    char pszDecoration4[256] = "none"; if(rpInit->rprt.fonts[3].lfUnderline) lstrcpy(pszDecoration4, "underline");
    char pszDecoration5[256] = "none"; if(rpInit->rprt.fonts[4].lfUnderline) lstrcpy(pszDecoration5, "underline");
    char pszDecoration6[256] = "none"; if(rpInit->rprt.fonts[5].lfUnderline) lstrcpy(pszDecoration6, "underline");
    
    if(rpInit->rprt.fonts[0].lfStrikeOut) lstrcat(pszDecoration1, " line-through");
    if(rpInit->rprt.fonts[1].lfStrikeOut) lstrcat(pszDecoration2, " line-through");
    if(rpInit->rprt.fonts[2].lfStrikeOut) lstrcat(pszDecoration3, " line-through");
    if(rpInit->rprt.fonts[3].lfStrikeOut) lstrcat(pszDecoration4, " line-through");
    if(rpInit->rprt.fonts[4].lfStrikeOut) lstrcat(pszDecoration5, " line-through");
    if(rpInit->rprt.fonts[5].lfStrikeOut) lstrcat(pszDecoration6, " line-through");
    
    char pszColor1[16]; wsprintf(pszColor1, "%lX", RGB(GetBValue(rpInit->rprt.colors[0]), GetGValue(rpInit->rprt.colors[0]), GetRValue(rpInit->rprt.colors[0])));
    char pszColor2[16]; wsprintf(pszColor2, "%lX", RGB(GetBValue(rpInit->rprt.colors[1]), GetGValue(rpInit->rprt.colors[1]), GetRValue(rpInit->rprt.colors[1])));
    char pszColor3[16]; wsprintf(pszColor3, "%lX", RGB(GetBValue(rpInit->rprt.colors[2]), GetGValue(rpInit->rprt.colors[2]), GetRValue(rpInit->rprt.colors[2])));
    char pszColor4[16]; wsprintf(pszColor4, "%lX", RGB(GetBValue(rpInit->rprt.colors[3]), GetGValue(rpInit->rprt.colors[3]), GetRValue(rpInit->rprt.colors[3])));
    char pszColor5[16]; wsprintf(pszColor5, "%lX", RGB(GetBValue(rpInit->rprt.colors[4]), GetGValue(rpInit->rprt.colors[4]), GetRValue(rpInit->rprt.colors[4])));
    char pszColor6[16]; wsprintf(pszColor6, "%lX", RGB(GetBValue(rpInit->rprt.colors[5]), GetGValue(rpInit->rprt.colors[5]), GetRValue(rpInit->rprt.colors[5])));
    
    while(lstrlen(pszColor1) < 6)
    {
        for(int i = lstrlen(pszColor1); i > 0; i--)
        {
            pszColor1[i] = pszColor1[i - 1];
            pszColor1[i - 1] = '0';
        }
    }
    pszColor1[6] = 0;
    while(lstrlen(pszColor2) < 6)
    {
        for(int i = lstrlen(pszColor2); i > 0; i--)
        {
            pszColor2[i] = pszColor2[i - 1];
            pszColor2[i - 1] = '0';
        }
    }
    pszColor2[6] = 0;
    while(lstrlen(pszColor3) < 6)
    {
        for(int i = lstrlen(pszColor3); i > 0; i--)
        {
            pszColor3[i] = pszColor3[i - 1];
            pszColor3[i - 1] = '0';
        }
    }
    pszColor3[6] = 0;
    while(lstrlen(pszColor4) < 6)
    {
        for(int i = lstrlen(pszColor4); i > 0; i--)
        {
            pszColor4[i] = pszColor4[i - 1];
            pszColor4[i - 1] = '0';
        }
    }
    pszColor4[6] = 0;
    while(lstrlen(pszColor5) < 6)
    {
        for(int i = lstrlen(pszColor5); i > 0; i--)
        {
            pszColor5[i] = pszColor5[i - 1];
            pszColor5[i - 1] = '0';
        }
    }
    pszColor5[6] = 0;
    while(lstrlen(pszColor6) < 6)
    {
        for(int i = lstrlen(pszColor6); i > 0; i--)
        {
            pszColor6[i] = pszColor6[i - 1];
            pszColor6[i - 1] = '0';
        }
    }
    pszColor6[6] = 0;
    
    Katalog * report = new Katalog;
    
    int iRange = 2 * rpInit->db->PobierzLiczbePozycji();
    SendMessage(rpInit->hProgress, PBM_SETRANGE32, 0, iRange);
    
    for(int i = 0; i < rpInit->db->PobierzLiczbePozycji(); i++)
    {
        DANE dane1; rpInit->db->PobierzDane(i, &dane1);
        rpInit->db = rpInit->db->OtworzPozycje(i);
        
        if(rpInit->db->PobierzLiczbePozycji() == 0)
        {
            DodajDoRaportu(report, rpInit->rprt.dane, &dane1, NULL, NULL);
        }
        else
        {
            for(int j = 0; j < rpInit->db->PobierzLiczbePozycji(); j++)
            {
                DANE dane2; rpInit->db->PobierzDane(j, &dane2);
                rpInit->db = rpInit->db->OtworzPozycje(j);
                
                if(rpInit->db->PobierzLiczbePozycji() == 0)
                {
                    DodajDoRaportu(report, rpInit->rprt.dane, &dane1, &dane2, NULL);
                }
                else
                {                        
                    for(int k = 0; k < rpInit->db->PobierzLiczbePozycji(); k++)
                    {
                        DANE dane3; rpInit->db->PobierzDane(k, &dane3);
                        DodajDoRaportu(report, rpInit->rprt.dane, &dane1, &dane2, &dane3);
                    }
                }
                rpInit->db = rpInit->db->ZamknijPozycje();
                
            }
        }
        
        rpInit->db = rpInit->db->ZamknijPozycje();
        SendMessage(rpInit->hProgress, PBM_STEPIT, 0, 0);
    }
    
    FILE * pFile = fopen(rpInit->rprt.pszFile, "wt");
    if(!pFile)
    {
        switch(GetLastError())
        {
            case ERROR_ACCESS_DENIED:
            {
                rpInit->error = _ERROR_FILEACCESSDENIED;
                break;
            }
            default:
            {
                rpInit->error = _ERROR_FILENOTCREATED;
                break;
            }
        }
        delete report;
        report = NULL;
        
        rpInit->result = false;
        rpInit->koniec = true;
        return 0;
    }
    
    fprintf(pFile, "<HTML><HEAD>\n");
    fprintf(pFile, "<META HTTP-EQUIV=\"Content-type\" CONTENT=\"text/html; charset=windows-1250\">\n");
    fprintf(pFile, "<META NAME=\"Author\" CONTENT=\"Katalogus 3.2\">\n");
    fprintf(pFile, "<TITLE>%s [ %s ]</TITLE>\n", Jezyk::pszInne[3], rpInit->rprt.pszTitle);
    fprintf(pFile, "<STYLE TYPE=\"text/css\"><!--\n");
    fprintf(pFile, "p.class0 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[0].lfFaceName, abs(rpInit->rprt.fonts[0].lfHeight), rpInit->rprt.fonts[0].lfWeight, pszStyle1, pszColor1, pszDecoration1);
    fprintf(pFile, "p.class1 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[1].lfFaceName, abs(rpInit->rprt.fonts[1].lfHeight), rpInit->rprt.fonts[1].lfWeight, pszStyle2, pszColor2, pszDecoration2);
    fprintf(pFile, "p.class2 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[2].lfFaceName, abs(rpInit->rprt.fonts[2].lfHeight), rpInit->rprt.fonts[2].lfWeight, pszStyle3, pszColor3, pszDecoration3);
    fprintf(pFile, "p.class3 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[3].lfFaceName, abs(rpInit->rprt.fonts[3].lfHeight), rpInit->rprt.fonts[3].lfWeight, pszStyle4, pszColor4, pszDecoration4);
    fprintf(pFile, "p.class4 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[4].lfFaceName, abs(rpInit->rprt.fonts[4].lfHeight), rpInit->rprt.fonts[4].lfWeight, pszStyle5, pszColor5, pszDecoration5);
    fprintf(pFile, "p.class5 { font-family: %s; font-size: %ipx; font-weight: %i; font-style: %s; color: #%s; text-decoration: %s; }\n", rpInit->rprt.fonts[5].lfFaceName, abs(rpInit->rprt.fonts[5].lfHeight), rpInit->rprt.fonts[5].lfWeight, pszStyle6, pszColor6, pszDecoration6);
    fprintf(pFile, "--></STYLE>");
    fprintf(pFile, "</HEAD><BODY bgcolor=\"#FFFFFF\">\n");
    fprintf(pFile, "<CENTER>");
    fprintf(pFile, "<P class=\"class0\">%s [ %s ]</p><BR>\n", Jezyk::pszInne[4], rpInit->rprt.pszTitle);
    fprintf(pFile, "<TABLE BORDER=\"0\" WIDTH=\"100%%\" HEIGHT=\"1\">");
    fprintf(pFile, "\n<TR>");
    for(int i = 0; i < 3; i++)
    {
        if(rpInit->rprt.dane[i] > 0)
            fprintf(pFile, "<TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class1\">%s%s</P></TD>", (i > 0) ? rpInit->rprt.pszStop[i - 1] : "", rpInit->rprt.pszHeaders[i]);
    }
    fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">&nbsp;</P></TD>");
    
    
    report->db->SzeregujNazwa(NULL);
    for(int i = 0; i < report->db->PobierzLiczbePozycji(); i++)
    {
        DANE dane1; report->db->PobierzDane(i, &dane1);
        
        if (rpInit->rprt.options[0] && i > 0 && rpInit->rprt.kaskada[0]) fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">&nbsp;</P></TD>");
        fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">%s</P></TD>", dane1.nazwa);
        
        report->Otworz(i);
        report->db->SzeregujNazwa(NULL);
                
        for(int j = 0; j < report->db->PobierzLiczbePozycji(); j++)
        {
            DANE dane2; report->db->PobierzDane(j, &dane2);
            
            if(rpInit->rprt.options[1] && j > 0 && rpInit->rprt.kaskada[1]) fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">&nbsp;</P></TD>");
            if( j > 0) fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">%s</P></TD>", (rpInit->rprt.kaskada[0]) ? "" : dane1.nazwa);
            fprintf(pFile, "<TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class3\">%s%s</P></TD>", rpInit->rprt.pszStop[0], dane2.nazwa);
            
            report->Otworz(j);
            report->db->SzeregujNazwa(NULL);
            
            for(int k = 0; k < report->db->PobierzLiczbePozycji(); k++)
            {
                DANE dane3; report->db->PobierzDane(k, &dane3);
            
                if( k > 0) fprintf(pFile, "</TR>\n<TR><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class2\">%s</P></TD><TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class3\">%s%s</P></TD>", (rpInit->rprt.kaskada[0]) ? "" : dane1.nazwa, (rpInit->rprt.kaskada[1]) ? "" : rpInit->rprt.pszStop[0], (rpInit->rprt.kaskada[1]) ? "" : dane2.nazwa);
                fprintf(pFile, "<TD ALIGN=\"LEFT\" VALIGN=\"TOP\"><P CLASS=\"class4\">%s%s</P></TD>", rpInit->rprt.pszStop[1], dane3.nazwa);
            }
            
            report->Wstecz();
        }
        
        report->Wstecz();
        SendMessage(rpInit->hProgress, PBM_STEPIT, 0, 0);
    }
    
    fprintf(pFile, "</TR>\n</TABLE>");
    fprintf(pFile, "\n<P CLASS=\"class5\">Wygenerowany przy pomocy programu Katalogus 3.2.<br>Copyright (c) by Xamax Software Patryk Koz³owski. All rights reserved.</P>");
    fprintf(pFile, "\n</CENTER>");
    fprintf(pFile, "\n</BODY></HTML>");
    fclose(pFile);
    
    delete report;
    report = NULL;
    
    SendMessage(rpInit->hProgress, PBM_SETPOS, iRange, 0);
    
    rpInit->error = 0;
    rpInit->result = true;
    rpInit->koniec = true;
    return 0;
}
