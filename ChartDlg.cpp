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
    bool ret = images.size() > 0;
    int amountX, amountY;
    if (ret)
    {
        ret = false;
        if (images[0]->valuesX.size() > 0)
        {
            minValueX = images[0]->minValueX;
            maxValueX = images[0]->maxValueX;
            amountX = images[0]->valuesX.size();
            for (ChartImage* img : images)
            {
                if (minValueX > img->minValueX)
                    minValueX = img->minValueX;
                if (maxValueX < img->maxValueX)
                    maxValueX = img->maxValueX;
                if (amountX < img->valuesX.size())
                    amountX = img->valuesX.size();
            }
            rangeX = 0.0;
            if (maxValueX > 0.0 && minValueX < 0.0 || maxValueX < 0.0 && minValueX > 0.0)
                rangeX = abs(maxValueX - minValueX);
            else if (maxValueX != minValueX)
                rangeX = abs(maxValueX) + abs(minValueX);
            kX = rangeX > 0.0 ? float(height - 2 * gapX) / rangeX : 10.0;
            ret = true;
        }
        if (images[0]->valuesY.size() > 0) 
        {
            minValueY = images[0]->minValueY;
            maxValueY = images[0]->maxValueY;
            amountY = images[0]->valuesY.size();
            for (ChartImage* img : images)
            {
                if (minValueY > img->minValueY)
                    minValueY = img->minValueY;
                if (maxValueY < img->maxValueY)
                    maxValueY = img->maxValueY;
                if (amountY < img->valuesY.size())
                    amountY = img->valuesY.size();
            }
            rangeY = 0.0;
            if (maxValueY > 0.0 && minValueY < 0.0 || maxValueY < 0.0 && minValueY > 0.0)
                rangeY = abs(maxValueY - minValueY);
            else if (maxValueY != minValueY)
                rangeY = abs(maxValueY) + abs(minValueY);
            kY = rangeY > 0.0 ? float(height - 2 * gapY) / rangeY : 10.0;
            ret = true;
        }
    }
    if (amountX == amountY)
        amount = amountX;
    else
        amount = (amountX < amountY) ? amountX : amountY;
    //amount--;
    return ret;
}

void ChartDlg::DrawCoordinates(Gdiplus::Graphics* g)
{
    if (!GetMinMax())
        return;
    kX = double(bgnX + wight - 2 * gapX) / amount;
    x0 = bgnX + gapX;
    y0 = bgnY + gapY + maxValueY * kY;
    Pen grayPen(Color(255, 180, 180, 180));
    g->DrawRectangle(&grayPen, bgnX, bgnY, wight, height);
    PointF* p0, * p1;
    double x = x0, y = bgnY;
    for (int i = 0; i < amount; i++) // x,x,x
    {
        x += kX;
        p0 = new PointF(x, y + gapY);
        p1 = new PointF(x, y + height);
        g->DrawLine(&grayPen, *p0, *p1);
    }
    int step = 10 * int(rangeY / 100);
    for (double i = step; i < rangeY; i += step)//y,y,y
    {
        double dY = y0 - i;
        if (dY > bgnY)
        {
            p0 = new PointF(x0 + amount * kX, dY);
            p1 = new PointF(x0, dY);
            g->DrawLine(&grayPen, *p0, *p1);
        }
        dY = y0 + i;
        if (dY < bgnY + height)
        {
            p0 = new PointF(x0 + amount * kX, dY);
            p1 = new PointF(x0, dY);
            g->DrawLine(&grayPen, *p0, *p1);
        }
    }
    x = x0 + wight - gapX;
    y = bgnY;
    Pen blackPen(Color(255, 0, 0, 0));
    p0 = new PointF(x0, y);
    p1 = new PointF(x0, bgnY + height);
    g->DrawLine(&blackPen, *p0, *p1); // y
    p1 = new PointF(x0 - 5, y + 10);
    g->DrawLine(&blackPen, *p0, *p1);
    p1 = new PointF(x0 + 5, y + 10);
    g->DrawLine(&blackPen, *p0, *p1);
    p0 = new PointF(x, y0);
    p1 = new PointF(x0, y0);
    g->DrawLine(&blackPen, *p0, *p1); // x
    p1 = new PointF(x - 10, y0 - 5);
    g->DrawLine(&blackPen, *p0, *p1);
    p1 = new PointF(x - 10, y0 + 5);
    g->DrawLine(&blackPen, *p0, *p1);
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
    for (int i = 0; i <= amount; i++)
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
        if (dY < bgnY + height)
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
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    HWND hwnd = GetSafeHwnd();
    Gdiplus::Graphics g(hwnd);
    DrawCoordinates(&g);
    for (ChartImage* img : images)
        img->DrawImage(&g);
        //img->DrawImageXY();
    DrawLabels(&g);
}

