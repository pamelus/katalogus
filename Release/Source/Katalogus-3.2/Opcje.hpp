bool Opcje::Wczytaj()
{
    RKey rKey;
    rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_READ);
    
    rKey.WczytajRekordA("isSerialAutor", isSerialAutor);
    rKey.WczytajRekordA("isSerialAlbum", isSerialAlbum);
    rKey.WczytajRekordA("isSerialAudio", isSerialAudio);
    rKey.WczytajRekordA("isFolderAutor", isFolderAutor);
    rKey.WczytajRekordA("isFolderAlbum", isFolderAlbum);
    rKey.WczytajRekordA("isFolderAudio", isFolderAudio);
    rKey.WczytajRekordA("isWarningEnabled", isWarningEnabled);
    rKey.WczytajRekordA("isCommentEnabled", isCommentEnabled);
    rKey.WczytajRekordA("isMeshEnabled", isMeshEnabled);
    rKey.WczytajRekordA("isSizeEnabled", isSizeEnabled);
    
    rKey.WczytajRekordD("tColor1", &tColor1, sizeof(COLORREF));
    rKey.WczytajRekordD("tColor2", &tColor2, sizeof(COLORREF));
    rKey.WczytajRekordD("bColor1", &bColor1, sizeof(COLORREF));
    rKey.WczytajRekordD("bColor2", &bColor2, sizeof(COLORREF));
    rKey.WczytajRekordD("lFont1", &lFont1, sizeof(LOGFONT));
    rKey.WczytajRekordD("lFont2", &lFont2, sizeof(LOGFONT));
    
    rKey.Zamknij();
    return true;
}

bool Opcje::Zapisz()
{
    RKey rKey;
    rKey.Otworz(HKEY_CURRENT_USER, HKEY_MYKEY, KEY_WRITE);
    
    rKey.ZapiszRekordA("isSerialAutor", isSerialAutor);
    rKey.ZapiszRekordA("isSerialAlbum", isSerialAlbum);
    rKey.ZapiszRekordA("isSerialAudio", isSerialAudio);
    rKey.ZapiszRekordA("isFolderAutor", isFolderAutor);
    rKey.ZapiszRekordA("isFolderAlbum", isFolderAlbum);
    rKey.ZapiszRekordA("isFolderAudio", isFolderAudio);
    rKey.ZapiszRekordA("isWarningEnabled", isWarningEnabled);
    rKey.ZapiszRekordA("isCommentEnabled", isCommentEnabled);
    rKey.ZapiszRekordA("isMeshEnabled", isMeshEnabled);
    rKey.ZapiszRekordA("isSizeEnabled", isSizeEnabled);
    
    rKey.ZapiszRekordD("tColor1", &tColor1, sizeof(COLORREF));
    rKey.ZapiszRekordD("tColor2", &tColor2, sizeof(COLORREF));
    rKey.ZapiszRekordD("bColor1", &bColor1, sizeof(COLORREF));
    rKey.ZapiszRekordD("bColor2", &bColor2, sizeof(COLORREF));
    rKey.ZapiszRekordD("lFont1", &lFont1, sizeof(LOGFONT));
    rKey.ZapiszRekordD("lFont2", &lFont2, sizeof(LOGFONT));
    
    rKey.Zamknij();
    return true;
}
