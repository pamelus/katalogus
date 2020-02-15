#ifndef _DEBUG_
#define assert(x)
#else
#define assert(x) \
if(!(x)) \
{\
    char pszMessage[1024]; \
    wsprintf(pszMessage, "Warunek %s niespe³niony!\nPlik: %s\nLinia: %i\nZakoñczyæ dzia³anie programu?", #x, __FILE__, __LINE__); \
    if(MessageBox(0, pszMessage, "B³¹d assercji", MB_YESNO | MB_ICONERROR) == IDYES) abort(); \
}
#endif
