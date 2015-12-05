/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Resize Photo                               //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizePhoto.h"
#include "ResizePhotoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//-------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//-------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------
CResizePhotoDlg::CResizePhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizePhotoDlg::IDD, pParent)
	, m_filename(_T("../Picture/original.jpg"))
	, m_width(300)
	, m_height(400)
	, m_output(_T("../Picture/result.jpg"))
	, m_message(_T("Press 'Resize' Button."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//-------------------------------------------------------------------------------------
void CResizePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_output);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
}

//-------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CResizePhotoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RESIZE, &CResizePhotoDlg::OnBnClickedBtnResize)
	ON_EN_CHANGE(IDC_EDIT_FILENAME, &CResizePhotoDlg::OnEnChangeEditFilename)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CResizePhotoDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CResizePhotoDlg::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT, &CResizePhotoDlg::OnEnChangeEditOutput)
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------
BOOL CResizePhotoDlg::OnInitDialog()
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

//-------------------------------------------------------------------------------------
void CResizePhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//-------------------------------------------------------------------------------------
void CResizePhotoDlg::OnPaint()
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

//------------------------------------------------------------------------------------------------------
HCURSOR CResizePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------------------------------
void CResizePhotoDlg::OnBnClickedBtnResize()
{
	UpdateData(TRUE);

	m_message.Format(_T("Resizing is Running..."));
	UpdateData(FALSE);

	if (image != NULL)
		image.Destroy();

	HRESULT hResult = image.Load(m_filename);

	if (FAILED(hResult)) {
		m_message.Format(_T("Error : Can't Open Input File."));
		UpdateData(FALSE);
		return;
	}

	photoProcess->ResizePhoto(&image, m_width, m_height, m_output);
	
	m_message.Format(_T("Resizing is Completed..."));
	UpdateData(FALSE);
}

//------------------------------------------------------------------------------------------------------
void CResizePhotoDlg::OnEnChangeEditFilename()
{
	UpdateData(TRUE);
}

//------------------------------------------------------------------------------------------------------
void CResizePhotoDlg::OnEnChangeEditWidth()
{
	UpdateData(TRUE);
}

//------------------------------------------------------------------------------------------------------
void CResizePhotoDlg::OnEnChangeEditHeight()
{
	UpdateData(TRUE);
}

//------------------------------------------------------------------------------------------------------
void CResizePhotoDlg::OnEnChangeEditOutput()
{
	UpdateData(TRUE);
}
