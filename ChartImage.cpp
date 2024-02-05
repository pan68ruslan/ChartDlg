#include "pch.h"
#include "ChartImage.h"

ChartImage::ChartImage(ChartDlg *chart)
{
    pChartDlg = chart;
    color = Color(255, 0, 0, 0);
    /*Pen p(Color(255, 0, 0, 0), 1.0);
    imgPen = &p;//Gdiplus::Pen(Color(255, 0, 0, 0), 1.0);*/
    shiftX = 0.0f;
    shiftY = 0.0f;
    scale = 1.0f;
}

ChartImage::~ChartImage()
{
    pChartDlg = NULL;
    ClearValues();
}

void ChartImage::AddValue(double y)
{
    if (valuesY.size() == 0)
        minValueY = maxValueY = y;
    valuesY.push_back(y);
    if (y > maxValueY)
        maxValueY = y;
    if (y < minValueY)
        minValueY = y;
}

void ChartImage::AddValue(double x, double y)
{
    if (valuesX.size() == 0)
        minValueX = maxValueX = x;
    valuesX.push_back(x);
    if (x > maxValueX)
        maxValueY = x;
    if (x < minValueX)
        minValueX = x;
    //
    if (valuesY.size() == 0)
        minValueY = maxValueY = y;
    valuesY.push_back(y);
    if (y > maxValueY)
        maxValueY = y;
    if (y < minValueY)
        minValueY = y;
}

void ChartImage::ClearValues()
{
    valuesY.clear();
}

void ChartImage::InitImage(Color c, double w)
{
    color = c;
    width = w;
    //imgPen->SetColor(c);
    //imgPen->SetWidth(w);
    //imgPen->SetAlignment(PenAlignmentCenter);
    //imgPen->SetEndCap(LineCapRoundAnchor);
}

void ChartImage::DrawImage(Gdiplus::Graphics* g)
{
    Pen pen(color, width);
    pen.SetAlignment(PenAlignmentCenter);
    pen.SetEndCap(LineCapRoundAnchor);
    double x = pChartDlg->x0;
    PointF *p1 = NULL;
    PointF *p0 = NULL;
    for (double v : valuesY)
    {
        p1 = new PointF(x, pChartDlg->y0 - v * pChartDlg->kY);
        if (p0 != NULL)
            g->DrawLine(&pen, *p0, *p1);
        p0 = new PointF(*p1);
        x += pChartDlg->kX;
    }
}

void ChartImage::DrawImageXY(Gdiplus::Graphics* g)
{
    Pen pen(color, width);
    pen.SetAlignment(PenAlignmentCenter);
    pen.SetEndCap(LineCapRoundAnchor);
    double x = pChartDlg->x0;
    PointF* p1 = NULL;
    PointF* p0 = NULL;
    for (int i = 0; i < pChartDlg->amount; i++)
    {
        p1 = new PointF(pChartDlg->x0 + valuesX[i] * pChartDlg->kX, pChartDlg->y0 - valuesY[i] * pChartDlg->kY);
        if (p0 != NULL)
            g->DrawLine(&pen, *p0, *p1);
        p0 = new PointF(*p1);
        //x += pChartDlg->kX;
    }
}
