void ObslugaBledu()
{
    DWORD errCode = GetLastError();
    switch(errCode)
    {
        case _ERROR_CANCEL:
        {
            break;
        }
        case _ERROR_NOTCREATED:
        {
            MessageBox(hOkna, Jezyk::pszError[0], pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_OVERDRIVE:
        {
            MessageBeep(MB_OK);
            break;
        }
        case _ERROR_FILENOTCREATED:
        {
            MessageBox(hOkna, Jezyk::pszError[1], pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_FILENOTOPENED:
        {
            MessageBox(hOkna, Jezyk::pszError[2], pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_LANFILENOTOPENED:
        {
            MessageBox(hOkna, "Wyst¹pi³ b³¹d podczas otwierania pliku jêzykowego. U¿yte zostan¹ domyœlne ustawienia jêzykowe.", pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_FILEACCESSDENIED:
        {
            MessageBox(hOkna, Jezyk::pszError[3], pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_LANFILEACCESSDENIED:
        {
            MessageBox(hOkna, "Wyst¹pi³ b³¹d dostêpu do pliku jêzykowego. U¿yte zostan¹ domyœlne ustawienia jêzykowe.", pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
        case _ERROR_NOTDATABASE:
        {
            MessageBox(hOkna, Jezyk::pszError[4], pszProgramName, MB_OK | MB_ICONERROR);
            break;
        }
    }
    SetLastError(0);
}
