/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Rotate Photo                               //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RotatePhoto.h"
#include "RotatePhotoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//-----------------------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//-----------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------------------
CRotatePhotoDlg::CRotatePhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotatePhotoDlg::IDD, pParent)
	, m_inputfile(_T("../Picture/original.jpg"))
	, m_outputfile(_T("../Picture/result.jpg"))
	, m_degree(0)
	, m_message(_T("Press 'Rotate' Button."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTFILE, m_inputfile);
	DDX_Text(pDX, IDC_EDIT_OUTPUTFILE, m_outputfile);
	DDX_Radio(pDX, IDC_RADIO_90RIGHT, m_degree);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
}

//-----------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CRotatePhotoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ROTATE, &CRotatePhotoDlg::OnBnClickedBtnRotate)
	ON_BN_CLICKED(IDC_RADIO_90RIGHT, &CRotatePhotoDlg::OnBnClickedRadio90right)
	ON_BN_CLICKED(IDC_RADIO_90LEFT, &CRotatePhotoDlg::OnBnClickedRadio90left)
	ON_BN_CLICKED(IDC_RADIO_180, &CRotatePhotoDlg::OnBnClickedRadio180)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------------------
BOOL CRotatePhotoDlg::OnInitDialog()
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

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnPaint()
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

//-----------------------------------------------------------------------------------------------------
HCURSOR CRotatePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnBnClickedBtnRotate()
{
	UpdateData(TRUE);

	m_message.Format(_T("Rotating is Running..."));
	UpdateData(FALSE);

	if (image != NULL)
		image.Destroy();

	HRESULT hResult = image.Load(m_inputfile);

	if (FAILED(hResult)) {
		m_message.Format(_T("Error : Can't Open Input File."));
		UpdateData(FALSE);
		return;
	}

	photoProcess->RotatePhoto(&image, m_degree, m_outputfile);

	m_message.Format(_T("Rotating is Completed..."));
	UpdateData(FALSE);
}

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnBnClickedRadio90right()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnBnClickedRadio90left()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------------
void CRotatePhotoDlg::OnBnClickedRadio180()
{
	UpdateData(TRUE);
}
