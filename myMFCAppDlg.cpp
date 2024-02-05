// myMFCAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChartDlg.h"
#include "myMFCApp.h"
#include "myMFCAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CmyMFCAppDlg dialog
CmyMFCAppDlg::CmyMFCAppDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(CmyMFCAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
/*
#define IDD_MYMFCAPP_DIALOG				102
#define IDD_CHART_DIALOG                103
#define IDD_CHART_IMAGE                 104*/
void CmyMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDD_CHART_IMAGE, chart);
}

BEGIN_MESSAGE_MAP(CmyMFCAppDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// CmyMFCAppDlg message handlers
BOOL CmyMFCAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	dlgChart.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPEDWINDOW
		, CRect(10, 10, 700, 500), this);
	ChartImage* redImage = dlgChart.CreateImage();
	redImage->InitImage(Color(255, 255, 0, 0), 2.5);// red
	for (int i = 0; i <= 18; i++)
	{
		if (i < 10)
			redImage->AddValue(1.5 * i, 5.0 * i - 60);
		else if (i < 15)
			redImage->AddValue(1.5 * i, 20.0 * i - 50);
		else
			redImage->AddValue(1.5 * i, 10.0 * (5 - i) - 50);
	}
	ChartImage* greenImage = dlgChart.CreateImage();
	greenImage->InitImage(Color(255, 0, 255, 0), 2.5);// green
	for (int i = 0; i <= 15; i++)
	{
		if (i < 4)
			greenImage->AddValue(1.5 * (5.0 - i), -5.0f * i);
		else if (i < 10)
			greenImage->AddValue(1.5 * (5.0 - i), -10.0f * i + 10);
		else
			greenImage->AddValue(1.5 * (5.0 - i), -20.0f * (5 - i));
	}
	ChartImage* blueImage = dlgChart.CreateImage();
	blueImage->InitImage(Color(255, 0, 0, 255), 2.5);// blue
	//blueImage->scale = 1.0f;
	//for (int i = 0; i <= 15; i++)
	//{
	//    if (i < 4)
	//        blueImage->AddValue(-5.0f * i);
	//    else if (i < 10)
	//        blueImage->AddValue(-10.0f * i + 10);
	//    else
	//        blueImage->AddValue(-20.0f * (5 - i));
	//}
	for (int i = 0; i <= 15; i++)
	{
		if (i < 8)
			blueImage->AddValue(0.5 * i, 5.0 * i);
		else if (i < 12)
			blueImage->AddValue(0.5 * i, 10.0 * i + 10);
		else
			blueImage->AddValue(0.5 * i, 12.0 * (7 - i));
	}
	ShowWindow(SW_NORMAL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmyMFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CmyMFCAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		Graphics graphics(dc.m_hDC);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		//dlgChart.DrawImages();
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmyMFCAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

