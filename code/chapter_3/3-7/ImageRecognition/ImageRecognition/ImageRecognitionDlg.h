/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 1, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: OCR (Optical Character Recognition)        //
/////////////////////////////////////////////////////////////

#pragma once

#include "OCR.h"

//---------------------------------------------------------------------------------------------------
class CImageRecognitionDlg : public CDialog
{
public:

	COCR *ocr;
	CImage image;

	CString m_inputfile;
	CString m_outputfile;
	CString m_message;

	CImageRecognitionDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_IMAGERECOGNITION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnRunocr();
};
