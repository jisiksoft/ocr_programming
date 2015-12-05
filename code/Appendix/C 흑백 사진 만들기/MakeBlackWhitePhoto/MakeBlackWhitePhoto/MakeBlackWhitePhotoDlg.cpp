/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Make Black-White Photo                     //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeBlackWhitePhoto.h"
#include "MakeBlackWhitePhotoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//-----------------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//-----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------------
CMakeBlackWhitePhotoDlg::CMakeBlackWhitePhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeBlackWhitePhotoDlg::IDD, pParent)
	, m_inputfile(_T("../Picture/original.jpg"))
	, m_outputfile(_T("../Picture/result.jpg"))
	, m_message(_T("Press 'Make BlackWhite Photo' Button."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//-----------------------------------------------------------------------------------------------
void CMakeBlackWhitePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTFILE, m_inputfile);
	DDX_Text(pDX, IDC_EDIT_OUTPUTFILE, m_outputfile);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
}

//-----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMakeBlackWhitePhotoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_MAKEBLACKWHITE, &CMakeBlackWhitePhotoDlg::OnBnClickedBtnMakeblackwhite)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------------
BOOL CMakeBlackWhitePhotoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	photoProcess = new CPhotoProcess();

	return TRUE;
}

//-----------------------------------------------------------------------------------------------
void CMakeBlackWhitePhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//-----------------------------------------------------------------------------------------------
void CMakeBlackWhitePhotoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//-----------------------------------------------------------------------------------------------
HCURSOR CMakeBlackWhitePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------
void CMakeBlackWhitePhotoDlg::OnBnClickedBtnMakeblackwhite()
{
	UpdateData(TRUE);

	m_message.Format(_T("Making BlackWhite Photo is Running..."));
	UpdateData(FALSE);

	if (image != NULL)
		image.Destroy();

	HRESULT hResult = image.Load(m_inputfile);

	if (FAILED(hResult)) {
		::AfxMessageBox(_T("Error : Can't Open Input File."));
		UpdateData(FALSE);
		return;
	}

	photoProcess->MakeBlackWhitePhoto(&image, m_outputfile);

	m_message.Format(_T("Making BlackWhite Photo is Completed..."));
	UpdateData(FALSE);
}
