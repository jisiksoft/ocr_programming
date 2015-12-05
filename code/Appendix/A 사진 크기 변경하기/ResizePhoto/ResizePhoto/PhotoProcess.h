/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Resize Photo                               //
/////////////////////////////////////////////////////////////

#pragma once

//-------------------------------------------------------------------------------------
class CPhotoProcess
{
public:
	CPhotoProcess(void);
	~CPhotoProcess(void);

	void ResizePhoto(CImage *image, int width, int height, CString outputFile);
};
