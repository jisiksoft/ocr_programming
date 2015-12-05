/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Rotate Photo                               //
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
void CPhotoProcess::RotatePhoto(CImage *image, int degree, CString outputFile)
{
	CImage newImage;

	int width = image->GetWidth();
	int height = image->GetHeight();

	if (degree == 0) {
		newImage.Create(height, width, 32);

		for (int i=0; i<height; i++)
			for(int j=0; j<width; j++)
				newImage.SetPixel(i,j, image->GetPixel(width - j - 1, height - i - 1));

	} else if (degree == 1) {

		newImage.Create(height, width, 32);

		for (int i=0; i<height; i++)
			for(int j=0; j<width; j++)
				newImage.SetPixel(i,j, image->GetPixel(j, i));

	} else if (degree == 2) {

		newImage.Create(width, height, 32);

		for (int i=0; i<width; i++)
			for(int j=0; j<height; j++)
				newImage.SetPixel(i,j, image->GetPixel(width - i -1, height - j - 1));

	}

	newImage.Save(outputFile, Gdiplus::ImageFormatJPEG);
}
