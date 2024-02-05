// myMFCAppDlg.h : header file
//
#pragma once

// CmyMFCAppDlg dialog
class CmyMFCAppDlg : public CDialogEx
{
// Construction
public:
	CmyMFCAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMFCAPP_DIALOG };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
	HICON m_hIcon;
	ChartDlg dlgChart;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
