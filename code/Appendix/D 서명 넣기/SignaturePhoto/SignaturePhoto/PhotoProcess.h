/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Signature Photo                            //
/////////////////////////////////////////////////////////////

#pragma once

//-----------------------------------------------------------------------------------------------
class CPhotoProcess
{
public:
	CPhotoProcess(void);
	~CPhotoProcess(void);

	void SignPhoto(CImage *image, CImage *signature, int position, int width, int height, CString resultFile);
};
