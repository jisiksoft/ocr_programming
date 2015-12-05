/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Resize Photo                               //
/////////////////////////////////////////////////////////////

#pragma once

#include "PhotoProcess.h"

//-------------------------------------------------------------------------------------
class CResizePhotoDlg : public CDialog
{
public:
	CPhotoProcess *photoProcess;
	CImage image;

	CString m_filename;
	int m_width;
	int m_height;
	CString m_output;
	CString m_message;

	CResizePhotoDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_RESIZEPHOTO_DIALOG };

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
	afx_msg void OnBnClickedBtnResize();
	afx_msg void OnEnChangeEditFilename();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditOutput();
};
