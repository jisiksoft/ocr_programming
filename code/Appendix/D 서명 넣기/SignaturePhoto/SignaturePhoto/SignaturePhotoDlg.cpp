/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 7, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Signature Photo                            //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SignaturePhoto.h"
#include "SignaturePhotoDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);

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
CSignaturePhotoDlg::CSignaturePhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignaturePhotoDlg::IDD, pParent)
	, m_picturefile(_T("../Picture/original.jpg"))
	, m_signaturefile(_T("../Picture/signature.jpg"))
	, m_width(300)
	, m_height(100)
	, m_position(3)
	, m_resultfile(_T("../Picture/result.jpg"))
	, m_message(_T("Press 'Sign Photo' Button."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PICTUREFILE, m_picturefile);
	DDX_Text(pDX, IDC_EDIT_SIGNATUREFILE, m_signaturefile);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Radio(pDX, IDC_RADIO_TOPLEFT, m_position);
	DDX_Text(pDX, IDC_EDIT_RESULTFILE, m_resultfile);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
}

//-----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSignaturePhotoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SIGNPHOTO, &CSignaturePhotoDlg::OnBnClickedBtnSignphoto)
	ON_EN_CHANGE(IDC_EDIT_PICTUREFILE, &CSignaturePhotoDlg::OnEnChangeEditPicturefile)
	ON_EN_CHANGE(IDC_EDIT_SIGNATUREFILE, &CSignaturePhotoDlg::OnEnChangeEditSignaturefile)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CSignaturePhotoDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CSignaturePhotoDlg::OnEnChangeEditHeight)
	ON_BN_CLICKED(IDC_RADIO_TOPLEFT, &CSignaturePhotoDlg::OnBnClickedRadioTopleft)
	ON_BN_CLICKED(IDC_RADIO_TOPRIGHT, &CSignaturePhotoDlg::OnBnClickedRadioTopright)
	ON_BN_CLICKED(IDC_RADIO_BOTTOMLEFT, &CSignaturePhotoDlg::OnBnClickedRadioBottomleft)
	ON_BN_CLICKED(IDC_RADIO_BOTTOMRIGHT, &CSignaturePhotoDlg::OnBnClickedRadioBottomright)
	ON_EN_CHANGE(IDC_EDIT_RESULTFILE, &CSignaturePhotoDlg::OnEnChangeEditResultfile)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------------
BOOL CSignaturePhotoDlg::OnInitDialog()
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
void CSignaturePhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CSignaturePhotoDlg::OnPaint()
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
HCURSOR CSignaturePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnBnClickedBtnSignphoto()
{
	UpdateData(TRUE);

	m_message.Format(_T("Signing Photo is Running..."));
	UpdateData(FALSE);

	if (image != NULL)
		image.Destroy();
	if (signature != NULL)
		signature.Destroy();

	HRESULT hResult = image.Load(m_picturefile);
	HRESULT hResult2 = signature.Load(m_signaturefile);

	if (FAILED(hResult) || FAILED(hResult2)) {
		m_message.Format(_T("Error : Can't Open Input File."));
		UpdateData(FALSE);
		return;
	}

	photoProcess->SignPhoto(&image, &signature, m_position, m_width, m_height, m_resultfile);

	m_message.Format(_T("Signing Photo is Completed..."));
	UpdateData(FALSE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnEnChangeEditPicturefile()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnEnChangeEditSignaturefile()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnEnChangeEditWidth()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnEnChangeEditHeight()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnBnClickedRadioTopleft()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnBnClickedRadioTopright()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnBnClickedRadioBottomleft()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnBnClickedRadioBottomright()
{
	UpdateData(TRUE);
}

//-----------------------------------------------------------------------------------------------
void CSignaturePhotoDlg::OnEnChangeEditResultfile()
{
	UpdateData(TRUE);
}
