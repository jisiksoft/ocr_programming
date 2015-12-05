
#pragma once


//---------------------------------------------------------------------------------------------------
class CImageRecognitionDlg : public CDialog
{
public:

	CImage image;

	CString m_inputfile;
	CString m_size;
	CString m_point;
	CString m_rgb;

	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CImageRecognitionDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_IMAGERECOGNITION_DIALOG };

protected:

	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
