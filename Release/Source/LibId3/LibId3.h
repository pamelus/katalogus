extern "C"
{
	UINT __declspec(dllexport)ID3GetAutor(const char * pszFile, char * pszAutor, UINT uSize);
	UINT __declspec(dllexport)ID3GetAlbum(const char * pszFile, char * pszAlbum, UINT uSize);
	UINT __declspec(dllexport)ID3GetTitle(const char * pszFile, char * pszTitle, UINT uSize);
	UINT __declspec(dllexport)ID3GetYear(const char * pszFile, char * pszYear, UINT uSize);
	UINT __declspec(dllexport)ID3GetTrack(const char * pszFile, char * pszTrack, UINT uSize);
	UINT __declspec(dllexport)ID3GetComment(const char * pszFile, char * pszComment, UINT uSize);
}