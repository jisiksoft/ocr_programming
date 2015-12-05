/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Signature Photo                            //
/////////////////////////////////////////////////////////////

#pragma once

#include "PhotoProcess.h"

//-----------------------------------------------------------------------------------------------
class CSignaturePhotoDlg : public CDialog
{
public:
	CPhotoProcess * photoProcess;
	CImage image, signature;

	CString m_picturefile;
	CString m_signaturefile;
	int m_width;
	int m_height;
	int m_position;
	CString m_resultfile;

	CSignaturePhotoDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SIGNATUREPHOTO_DIALOG };

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
	afx_msg void OnBnClickedBtnSignphoto();
	afx_msg void OnEnChangeEditPicturefile();
	afx_msg void OnEnChangeEditSignaturefile();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnBnClickedRadioTopleft();
	afx_msg void OnBnClickedRadioTopright();
	afx_msg void OnBnClickedRadioBottomleft();
	afx_msg void OnBnClickedRadioBottomright();
	afx_msg void OnEnChangeEditResultfile();
	CString m_message;
};
