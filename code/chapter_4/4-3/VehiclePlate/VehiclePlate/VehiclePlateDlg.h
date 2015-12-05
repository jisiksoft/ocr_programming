/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Feb 28, 2015                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Recognize Vehicle Plate                    //
/////////////////////////////////////////////////////////////

#pragma once

#include "OCR.h"

//---------------------------------------------------------------------------------------------------
class CVehiclePlateDlg : public CDialog
{
public:

	COCR *ocr;
	CImage image;

	CString m_inputfile;
	CString m_message;

	CVehiclePlateDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_VEHICLEPLATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOcr();
};
