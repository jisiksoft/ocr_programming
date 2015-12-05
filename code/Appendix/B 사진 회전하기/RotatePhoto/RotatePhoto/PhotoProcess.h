/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Rotate Photo                               //
/////////////////////////////////////////////////////////////

#pragma once

class CPhotoProcess
{
public:
	CPhotoProcess(void);
	~CPhotoProcess(void);
	
	void RotatePhoto(CImage *image, int degree, CString outputFile);
};
