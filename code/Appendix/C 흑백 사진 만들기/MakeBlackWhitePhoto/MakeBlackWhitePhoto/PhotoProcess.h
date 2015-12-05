/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Make Black-White Photo                     //
/////////////////////////////////////////////////////////////

#pragma once

//-----------------------------------------------------------------------------------------------
class CPhotoProcess
{
public:
	CPhotoProcess(void);
	~CPhotoProcess(void);
	
	void MakeBlackWhitePhoto(CImage *image, CString outputFile);
};
