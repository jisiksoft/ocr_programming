
#include "stdafx.h"
#include "ImageRecognition.h"
#include "ImageRecognitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------------------------------
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------------
CImageRecognitionDlg::CImageRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageRecognitionDlg::IDD, pParent)
	, m_inputfile(_T(""))
	, m_point(_T(""))
	, m_rgb(_T(""))
	, m_size(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTFILE, m_inputfile);
	DDX_Text(pDX, IDC_STATIC_POINT, m_point);
	DDX_Text(pDX, IDC_STATIC_RGB, m_rgb);
	DDX_Text(pDX, IDC_STATIC_SIZE, m_size);
}

//---------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CImageRecognitionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RUN, &CImageRecognitionDlg::OnBnClickedBtnRun)
	ON_WM_LBUTTONDOWN()
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

	m_inputfile.Format(_T("standardimage.jpg"));

	m_size.Format(_T("Size: - x -"));
	m_point.Format(_T("Point(-,-)"));
	m_rgb.Format(_T("RGB(-,-,-)"));

	UpdateData(FALSE);

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
void CImageRecognitionDlg::OnBnClickedBtnRun()
{
	UpdateData(TRUE);

	if (image != NULL)
		image.Destroy();

	HRESULT hResult = image.Load(m_inputfile);

	if (FAILED(hResult))
	{
		CString strError = TEXT("ERROR : ");
		strError += m_inputfile + TEXT(" 파일을 열수가 없습니다.");
		::AfxMessageBox(strError);
		return;
	}

	m_size.Format(_T("Size: %d x %d"), image.GetWidth(), image.GetHeight());
	UpdateData(FALSE);

	CClientDC dc(this);
	image.BitBlt(dc.m_hDC, 0, 0);
}

//---------------------------------------------------------------------------------------------------
void CImageRecognitionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (image == NULL)
		return;

	if (point.x < image.GetWidth() && point.y < image.GetHeight()) {

		COLORREF rgb = image.GetPixel(point.x,point.y);

		m_point.Format(_T("Point(%d,%d)"), point.x, point.y);
		m_rgb.Format(_T("RGB(%d,%d,%d)"), GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));

		UpdateData(FALSE);
	}

	CDialog::OnLButtonDown(nFlags, point);
}
