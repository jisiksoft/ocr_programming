/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 1, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: OCR (Optical Character Recognition)        //
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageRecognition.h"
#include "ImageRecognitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

//---------------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//---------------------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//---------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------------
CImageRecognitionDlg::CImageRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageRecognitionDlg::IDD, pParent)
	, m_inputfile(_T("aesop_1.png"))
	, m_outputfile(_T("aesop_1.txt"))
	, m_message(_T("Press 'OCR' button."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTFILE, m_inputfile);
	DDX_Text(pDX, IDC_EDIT_OUTPUTFILE, m_outputfile);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_message);
}

//---------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CImageRecognitionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RUNOCR, &CImageRecognitionDlg::OnBnClickedBtnRunocr)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------------
BOOL CImageRecognitionDlg::OnInitDialog()
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

	ocr = new COCR();

	return TRUE;
}

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::OnPaint()
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

//---------------------------------------------------------------------------------------------------
HCURSOR CImageRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::OnBnClickedBtnRunocr()
{
	UpdateData(TRUE);

	m_message.Format(_T("OCR is Running..."));
	UpdateData(FALSE);

	if (image != NULL)
		image.Destroy();

	HRESULT hResult = image.Load(m_inputfile);

	if (FAILED(hResult)) {
		m_message.Format(_T("Error : Can't Open Input File."));
		UpdateData(FALSE);
		return;
	}

	//---------------------------- OCR --------------------------
	ocr->RunOCR(&image, m_outputfile, 50);

	m_message.Format(_T("OCR is Completed..."));
	UpdateData(FALSE);
}
