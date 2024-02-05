#pragma once
#include <afxwin.h>
#include <Windows.h>
#include <gdiplus.h>
#include <vector>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

#include "ChartImage.h"
//#include "ChartDlg.h"
#define CHART_CLASSNAME    _T("ChartDlg_MFCCtrl")

class ChartImage;

class ChartDlg : public CWnd
{
private:
    //BOOL DoRedraw;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    std::vector<ChartImage*> images;
    bool GetMinMax();
    void DrawCoordinates(Gdiplus::Graphics*);
    void DrawLabels(Gdiplus::Graphics*);
    void DrawText(Gdiplus::Graphics*, const WCHAR*, size_t, double, double, Gdiplus::StringAlignment);
public:
    ChartDlg();
    ~ChartDlg();
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHART_DIALOG };
#endif
protected:
    BOOL RegisterWindowClass();
    afx_msg void OnPaint();
    //virtual void PreSubclassWindow();
    //virtual BOOL OnInitDialog();
    //afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    //afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    //Gdiplus::Graphics* pGr;
    int bgnX, bgnY, wight, height, amount;
    double x0, y0, gapX, gapY, minValueX, maxValueX, minValueY, maxValueY, kX, kY, rangeX, rangeY;

    ChartImage* CreateImage();
    void ClearImages();
    void DrawImages();
    //afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //virtual void PreSubclassWindow();
};

