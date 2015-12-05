/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Signature Photo                            //
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
void CPhotoProcess::SignPhoto(CImage *image, CImage *signature, int position, int width, int height, CString resultFile)
{
	CImage newSignature;

	newSignature.Create(width, height, 32);

	float xRate, yRate;
	xRate = (float)(signature->GetWidth()) / width;
	yRate = (float)(signature->GetHeight()) / height;	

	for (int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			newSignature.SetPixel(i,j, signature->GetPixel((xRate * i), (yRate * j)));

	int widthImage = image->GetWidth();
	int heightImage = image->GetHeight();

	int startX, startY;

	if (position == 0) {
		startX = 30;
		startY = 30;
	} else if (position == 1) {
		startX = widthImage - width - 30;
		startY = 30;
	} else if (position == 2) {
		startX = 30;
		startY = heightImage - height - 30;
	} else if (position == 3) {
		startX = widthImage - width - 30;
		startY = heightImage - height - 30;
	}

	COLORREF rgb;

	int R = 255;
	int G = 255;
	int B = 255;
	int gap = 100;

	for (int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			rgb = newSignature.GetPixel(x,y);

			if (!((GetRValue(rgb) > (R-gap) && GetRValue(rgb) < (R+gap)) &&
				(GetGValue(rgb) > (G-gap) && GetGValue(rgb) < (G+gap)) &&
				(GetBValue(rgb) > (B-gap) && GetBValue(rgb) < (B+gap)))) {

					image->SetPixel(x+startX, y+startY, newSignature.GetPixel(x,y));
			}
		}
	}

	image->Save(resultFile, Gdiplus::ImageFormatJPEG);
}
