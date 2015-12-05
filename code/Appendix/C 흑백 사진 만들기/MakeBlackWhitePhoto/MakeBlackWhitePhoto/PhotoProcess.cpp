/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Make Black-White Photo                     //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PhotoProcess.h"

//-----------------------------------------------------------------------------------------------
CPhotoProcess::CPhotoProcess(void)
{
}

//-----------------------------------------------------------------------------------------------
CPhotoProcess::~CPhotoProcess(void)
{
}
	
//-----------------------------------------------------------------------------------------------
void CPhotoProcess::MakeBlackWhitePhoto(CImage *image, CString outputFile)
{
	CImage newImage;
	COLORREF rgb;

	int width = image->GetWidth();
	int height = image->GetHeight();

	newImage.Create(width, height, 32);

	for (int i=0; i<width; i++) {
		for(int j=0; j<height; j++) {

			rgb = image->GetPixel(i, j);

			BYTE byGray = (GetRValue(rgb) * 30 + 
						   GetGValue(rgb) * 59 +
						   GetBValue(rgb) * 11) / 100;
	
			rgb = RGB(byGray, byGray, byGray);

			newImage.SetPixel(i, j, rgb);
		}
	}

	newImage.Save(outputFile, Gdiplus::ImageFormatJPEG);
}
