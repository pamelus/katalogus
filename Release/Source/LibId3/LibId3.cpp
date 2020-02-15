#define ID3LIB_LINKOPTION 1
#include <windows.h>
#include <id3/tag.h>
#include "libid3.h"

UINT ID3GetAutor(const char * pszFile, char * pszAutor, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_LEADARTIST))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszAutor, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszAutor, "", uSize);
		pszAutor[0] = 0;
    }
    myTag.Clear();
	return 0;
}

UINT ID3GetAlbum(const char * pszFile, char * pszAlbum, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_ALBUM))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszAlbum, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszAlbum, "", uSize);
		pszAlbum[0] = 0;
    }
    myTag.Clear();
	return 0;
}

UINT ID3GetTitle(const char * pszFile, char * pszTitle, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_TITLE))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszTitle, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszTitle, "", uSize);
		pszTitle[0] = 0;

    }
    myTag.Clear();
	return 0;
}

UINT ID3GetYear(const char * pszFile, char * pszYear, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_YEAR))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszYear, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszYear, "", uSize);
    }
    myTag.Clear();
	return 0;
}

UINT ID3GetComment(const char * pszFile, char * pszComment, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_COMMENT))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszComment, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszComment, "", uSize);
    }
    myTag.Clear();
	return 0;
}


UINT ID3GetTrack(const char * pszFile, char * pszTrack, UINT uSize)
{
	ID3_Tag myTag;
    myTag.Link(pszFile);
    
    ID3_Frame * myFrame;
    if(myFrame = myTag.Find(ID3FID_TRACKNUM))
    {
        myFrame->Field(ID3FN_TEXT).Get(pszTrack, uSize);
		myFrame->Clear();
    }
    else
    {
        lstrcpyn(pszTrack, "", uSize);
    }
    myTag.Clear();
	return 0;
}
