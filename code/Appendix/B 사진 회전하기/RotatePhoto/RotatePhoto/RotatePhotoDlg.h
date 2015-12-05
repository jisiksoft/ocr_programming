/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Rotate Photo                               //
/////////////////////////////////////////////////////////////

#pragma once

#include "PhotoProcess.h"

//-------------------------------------------------------------------------------------
class CRotatePhotoDlg : public CDialog
{
public:
	CPhotoProcess * photoProcess;
	CImage image;

	CString m_inputfile;
	CString m_outputfile;
	int m_degree;
	CString m_message;

	CRotatePhotoDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_ROTATEPHOTO_DIALOG };

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
	afx_msg void OnBnClickedBtnRotate();
	afx_msg void OnBnClickedRadio90right();
	afx_msg void OnBnClickedRadio90left();
	afx_msg void OnBnClickedRadio180();
};
