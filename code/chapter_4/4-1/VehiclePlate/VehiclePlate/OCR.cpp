/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Feb 28, 2015                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Recognize Vehicle Plate                    //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OCR.h"

//---------------------------------------------------------------------------------------------------
COCR::COCR(void) {
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void) {
}

//---------------------------------------------------------------------------------------------------
// Standard Image Data를 만든다.
void COCR::RunOCR(CImage *newImage) {

	image = ResizeInBlackWhiteImage(newImage);
	
	image->Save("resizeBlackWhite.jpg", Gdiplus::ImageFormatJPEG);
}

//---------------------------------------------------------------------------------------------------
CImage * COCR::ResizeInBlackWhiteImage(CImage *newImage) {

	static CImage image;

	COLORREF rgb;
	BYTE byte;

	int width = 400;
	int height = 300;

	float xRate = (float)(newImage->GetWidth()) / width;
	float yRate = (float)(newImage->GetHeight()) / height;

	if (image != NULL)
		image.Destroy();

	image.Create(width, height, 32);

	for (int i=0; i<width; i++) {
		for(int j=0; j<height; j++) {

			rgb = newImage->GetPixel((int)(xRate * i), (int)(yRate * j));

			byte = (GetRValue(rgb) * 30 + 
					GetGValue(rgb) * 59 +
					GetBValue(rgb) * 11) / 100;

			rgb = RGB(byte, byte, byte);

			image.SetPixel(i,j, rgb);
		}
	}
	
	return &image;
}
