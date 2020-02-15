class Opcje
{
    public:
        static bool Wczytaj();
        static bool Zapisz();
        
        static bool IsSerialAutor() { return isSerialAutor; }
        static bool IsSerialAlbum() { return isSerialAlbum; }
        static bool IsSerialAudio() { return isSerialAudio; }
        static bool IsFolderAutor() { return isFolderAutor; }
        static bool IsFolderAlbum() { return isFolderAlbum; }
        static bool IsFolderAudio() { return isFolderAudio; }
        static bool IsWarningEnabled() { return isWarningEnabled; }
        static bool IsCommentEnabled() { return isCommentEnabled; }
        static bool IsMeshEnabled() { return isMeshEnabled; }
        static bool IsSizeEnabled() { return isSizeEnabled; }
        
        static bool SetSerialAutor(bool value) { isSerialAutor = value; return true; }
        static bool SetSerialAlbum(bool value) { isSerialAlbum = value; return true; }
        static bool SetSerialAudio(bool value) { isSerialAudio = value; return true; }
        static bool SetFolderAutor(bool value) { isFolderAutor = value; return true; }
        static bool SetFolderAlbum(bool value) { isFolderAlbum = value; return true; }
        static bool SetFolderAudio(bool value) { isFolderAudio = value; return true; }
        static bool SetWarningEnabled(bool value) { isWarningEnabled = value; return true; }
        static bool SetCommentEnabled(bool value) { isCommentEnabled = value; return true; }
        static bool SetMeshEnabled(bool value) { isMeshEnabled = value; return true; }
        static bool SetSizeEnabled(bool value) { isSizeEnabled = value; return true; }
        
        static COLORREF GetTextColor1() { return tColor1; }
        static COLORREF GetTextColor2() { return tColor2; }
        static COLORREF GetBkColor1() { return bColor1; }
        static COLORREF GetBkColor2() { return bColor2; }
        
        static bool SetTextColor1(COLORREF value) { tColor1 = value; return true; }
        static bool SetTextColor2(COLORREF value) { tColor2 = value; return true; }
        static bool SetBkColor1(COLORREF value) { bColor1 = value; return true; }
        static bool SetBkColor2(COLORREF value) { bColor2 = value; return true; }
        
        static bool GetFont1(LOGFONT * value) { CopyMemory(value, &lFont1, sizeof(LOGFONT)); }
        static bool GetFont2(LOGFONT * value) { CopyMemory(value, &lFont2, sizeof(LOGFONT)); }
        
        static bool SetFont1(const LOGFONT * value) { CopyMemory(&lFont1, value, sizeof(LOGFONT)); }
        static bool SetFont2(const LOGFONT * value) { CopyMemory(&lFont2, value, sizeof(LOGFONT)); }
           
    private:
        static bool isSerialAutor;
        static bool isSerialAlbum;
        static bool isSerialAudio;
        static bool isFolderAutor;
        static bool isFolderAlbum;
        static bool isFolderAudio;
        static bool isWarningEnabled;
        static bool isCommentEnabled;
        static bool isMeshEnabled;
        static bool isSizeEnabled;
        
        static COLORREF tColor1;
        static COLORREF tColor2;
        static COLORREF bColor1;
        static COLORREF bColor2;
        
        static LOGFONT lFont1;
        static LOGFONT lFont2;
};

// Wartoœci domyœlne
bool Opcje::isSerialAutor = false;;
bool Opcje::isSerialAlbum = false;;
bool Opcje::isSerialAudio = true;
bool Opcje::isFolderAutor = false;
bool Opcje::isFolderAlbum = false;
bool Opcje::isFolderAudio = false;
bool Opcje::isWarningEnabled = true;
bool Opcje::isCommentEnabled = true;
bool Opcje::isMeshEnabled = true;
bool Opcje::isSizeEnabled = true;
COLORREF Opcje::tColor1 = 0x000000;
COLORREF Opcje::tColor2 = 0x000000;
COLORREF Opcje::bColor1 = 0xFFFFFF;
COLORREF Opcje::bColor2 = 0x0ACFF5;
LOGFONT Opcje::lFont1 = { 8, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif" };
LOGFONT Opcje::lFont2 = { 8, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif" };
