/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Resize Photo                               //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PhotoProcess.h"

//-------------------------------------------------------------------------------------
CPhotoProcess::CPhotoProcess(void)
{
}

//-------------------------------------------------------------------------------------
CPhotoProcess::~CPhotoProcess(void)
{
}
	
//-----------------------------------------------------------------------------------------------
void CPhotoProcess::ResizePhoto(CImage *image, int width, int height, CString outputFile)
{
	CImage newImage;

	if (width == 0 && height == 0) {
		AfxMessageBox(_T("Error: 사진의 크기를 다시 설정하세요."));
		return;
	}

	int widthImage = image->GetWidth();
	int heightImage = image->GetHeight();

	if (width == 0)
		width = (int)((widthImage * height) / heightImage);
	else if (height == 0)
		height = (int)((heightImage * width) / widthImage);

	float xRate, yRate;

	xRate = (float)(widthImage) / (width);
	yRate = (float)(heightImage) / (height);

	newImage.Create(width, height, 32);

	for (int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			newImage.SetPixel(i,j, image->GetPixel((xRate * i), (yRate * j)));

	newImage.Save(outputFile, Gdiplus::ImageFormatJPEG);
}
