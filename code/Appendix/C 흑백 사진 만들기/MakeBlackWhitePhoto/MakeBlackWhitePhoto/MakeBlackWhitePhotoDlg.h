/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Make Black-White Photo                     //
/////////////////////////////////////////////////////////////

#pragma once

#include "PhotoProcess.h"

//-----------------------------------------------------------------------------------------------
class CMakeBlackWhitePhotoDlg : public CDialog
{
public:
	CPhotoProcess * photoProcess;
	CImage image;

	CString m_inputfile;
	CString m_outputfile;

	CMakeBlackWhitePhotoDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_MAKEBLACKWHITEPHOTO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnMakeblackwhite();
	CString m_message;
};
