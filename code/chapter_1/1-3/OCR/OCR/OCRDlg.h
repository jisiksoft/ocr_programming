
#include "ImageRecognition.h"

#pragma once


// COCRDlg ��ȭ ����
class COCRDlg : public CDialog
{
// �����Դϴ�.
public:

	CImageRecognition *imageRecognition;

	COCRDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OCR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_charFirst;
	CString m_charSecond;
	afx_msg void OnBnClickedBtnRun();
};
