#include "pch.h"
//#include "stdafx.h"
#include "ChartDlg.h"

BEGIN_MESSAGE_MAP(ChartDlg, CWnd)
    //ON_WM_CREATE()
    ON_WM_PAINT()
    //ON_WM_QUERYDRAGICON()
    //ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

ChartDlg::ChartDlg()
{
     RegisterWindowClass();
}

ChartDlg::~ChartDlg()
{
    ClearImages();
    GdiplusShutdown(gdiplusToken);
}

BOOL ChartDlg::RegisterWindowClass()
{
    WNDCLASS wndClass;
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!(::GetClassInfo(hInst, CHART_CLASSNAME, &wndClass)))
    {
        // otherwise we need to register a new class
        wndClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = ::DefWindowProc;
        wndClass.cbClsExtra = wndClass.cbWndExtra = 0;
        wndClass.hInstance = hInst;
        wndClass.hIcon = NULL;
        wndClass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = CHART_CLASSNAME;
        if (!AfxRegisterClass(&wndClass))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
    return TRUE;
}

void ChartDlg::OnPaint()
{
    DrawImages();
}

BOOL ChartDlg::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    bgnX = 10;
    bgnY = 10;
    gapX = 70.0;
    gapY = 20.0;
    wight = rect.right - 50;
    height = rect.bottom - 70;
    return CWnd::Create( CHART_CLASSNAME, _T("Chart dialog"), dwStyle, rect, pParentWnd, 1);
}

bool ChartDlg::GetMinMax()
{
    if (images.size() <= 0)
        return false;
    amountX = images[0]->valuesX.size();
    minValueX = images[0]->minX;
    maxValueX = images[0]->maxX;
    amountY = images[0]->valuesY.size();
    minValueY = images[0]->minY;
    maxValueY = images[0]->maxY;
    for (ChartImage* img : images)
    {
        if (amountX < img->valuesX.size())
            amountX = img->valuesX.size();
        if (minValueX > img->minX)
            minValueX = img->minX;
        if (maxValueX < img->maxX)
            maxValueX = img->maxX;
        if (amountY < img->valuesY.size())
            amountY = img->valuesY.size();
        if (minValueY > img->minY)
            minValueY = img->minY;
        if (maxValueY < img->maxY)
            maxValueY = img->maxY;
    }
    rangeX = 0.0;
    if (maxValueX > 0.0 && minValueX < 0.0 || maxValueX < 0.0 && minValueX > 0.0)
        rangeX = abs(maxValueX - minValueX);
    else
        rangeX = abs(maxValueX) + abs(minValueX);
    kX = rangeX > 0.0 ? double(wight - 2 * gapX) / rangeX : 10.0;
    x0 = bgnX + gapX - minValueX * kX;
    rangeY = 0.0;
    if (maxValueY > 0.0 && minValueY < 0.0 || maxValueY < 0.0 && minValueY > 0.0)
        rangeY = abs(maxValueY - minValueY);
    else
        rangeY = abs(maxValueY) + abs(minValueY);
    kY = rangeY > 0.0 ? double(height - 2 * gapY) / rangeY : 10.0;
    y0 = bgnY + gapY + maxValueY * kY;
    return true;
}

void ChartDlg::DrawCoordinates(Gdiplus::Graphics* g)
{
    Pen bluePen(Color(255, 180, 0, 180));
    Pen grayPen(Color(255, 180, 180, 180));
    g->DrawRectangle(&grayPen, bgnX, bgnY, wight, height);
    PointF p0, p1;
    double stepX = double(bgnX + wight - 2 * gapX) / amountX;
    double x = bgnX + gapX, y = bgnY;
    for (double dV = stepX; dV < rangeX; dV += stepX) // x,x,x
    {
        double dX = x0 - dV;
        p0 = PointF(x, y + gapY);
        p1 = PointF(x, y + height);
        g->DrawLine(&bluePen, p0, p1);
    }
    int stepY = 10 * int(rangeY / 100);
    for (double dV = stepY; dV < rangeY; dV += stepY)//y,y,y
    {
        double dY = y0 - dV;
        if (dY > bgnY)
        {
            p0 = PointF(bgnX + wight, dY);
            p1 = PointF(bgnX, dY);
            g->DrawLine(&grayPen, p0, p1);
        }
        dY = y0 + dV;
        if (dY < bgnY + height)
        {
            p0 = PointF(bgnX + wight, dY);
            p1 = PointF(bgnX, dY);
            g->DrawLine(&grayPen, p0, p1);
        }
    }
    Pen blackPen(Color(255, 0, 0, 0));
    x = bgnX + wight;
    y = bgnY;
    p0 = PointF(bgnX, y0);
    p1 = PointF(x, y0);
    g->DrawLine(&blackPen, p0, p1); // x
    p0 = PointF(x - 10, y0 - 5);
    g->DrawLine(&blackPen, p0, p1);
    p0 = PointF(x - 10, y0 + 5);
    g->DrawLine(&blackPen, p0, p1);
    p0 = PointF(x0, y);
    p1 = PointF(x0, bgnY + height);
    g->DrawLine(&blackPen, p0, p1); // y
    p1 = PointF(x0 - 5, y + 10);
    g->DrawLine(&blackPen, p0, p1);
    p1 = PointF(x0 + 5, y + 10);
    g->DrawLine(&blackPen, p0, p1);
}

void ChartDlg::DrawText(Gdiplus::Graphics* g, const WCHAR* s, size_t l, double x, double y, Gdiplus::StringAlignment aligment = Gdiplus::StringAlignmentNear)
{
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 10, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    StringFormat strFormat;
    strFormat.SetAlignment(aligment);
    strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    Pen pen(Color(255, 220, 220, 220));
    SolidBrush brush(Color(255, 0, 0, 0));
    RectF rect(x, y, 50, 20);
    g->DrawRectangle(&pen, rect);
    g->DrawString(s, l, &font, rect, &strFormat, &brush);
}

void ChartDlg::DrawLabels(Gdiplus::Graphics* g)
{
    PointF      pointF(bgnX, bgnY);
    SolidBrush  blackBrush(Color(255, 0, 0, 0));
    WCHAR str[10];
    double x = x0 - 10;
    for (int i = 0; i < amountX; i++)
    {
        swprintf_s(str, 10, L"%d", i);
        DrawText(g, str, wcslen(str), x, y0);
        x += kX;
    }
    x = bgnX + 18;
    double y = y0 - 6;
    double step = 10.0f * int(rangeY / 100);
    for (double i = step; i < rangeY; i += step)
    {
        double dY = y - i;
        if (dY > bgnY + gapY)
        {
            swprintf_s(str, 10, L"%5.1f", i);
            DrawText(g, str, wcslen(str), x, dY, Gdiplus::StringAlignmentFar);
        }
        dY = y + i;
        if (dY < bgnY + height - gapY)
        {
            swprintf_s(str, 10, L"%5.1f", -1 * i);
            DrawText(g, str, wcslen(str), x, dY, Gdiplus::StringAlignmentFar);
        }
    }
}

void ChartDlg::ClearImages()
{
    if (images.size() > 0)
    {
        for (ChartImage* img : images)
            delete img;
        images.clear();
    }
}

ChartImage* ChartDlg::CreateImage()
{
    ChartImage* image = new ChartImage(this);
    images.push_back(image);
    return image;
}

void ChartDlg::DrawImages()
{
    if (!GetMinMax())
        return;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    HWND hwnd = GetSafeHwnd();
    Gdiplus::Graphics g(hwnd);
    DrawCoordinates(&g);
    for (ChartImage* img : images)
        //img->DrawImage(&g);
        img->DrawImageXY(&g);
    DrawLabels(&g);
}

