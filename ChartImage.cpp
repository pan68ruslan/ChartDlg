#include "pch.h"
#include "ChartImage.h"

ChartImage::ChartImage(ChartDlg *chart)
{
    pChartDlg = chart;
    color = Color(255, 0, 0, 0);
    minX = 0.0;
    maxX = 0.0;
    minY = 0.0;
    maxY = 0.0;
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
        minY = maxY = y;
    valuesY.push_back(y);
    if (y > maxY)
        maxY = y;
    if (y < minY)
        minY = y;
}

void ChartImage::AddValue(double x, double y)
{
    if (valuesX.size() == 0)
        minX = maxX = x;
    valuesX.push_back(x);
    if (x > maxX)
        maxX = x;
    if (x < minX)
        minX = x;
    //
    if (valuesY.size() == 0)
        minY = maxY = y;
    valuesY.push_back(y);
    if (y > maxY)
        maxY = y;
    if (y < minY)
        minY = y;
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
    PointF p1;
    PointF p0 = PointF(x, pChartDlg->y0 - valuesY[0] * pChartDlg->kY);
    for (int i = 1; i < valuesY.size(); i++)
    {
        p1 = PointF(x, pChartDlg->y0 - valuesY[i] * pChartDlg->kY);
        g->DrawLine(&pen, p0, p1);
        p0 = PointF(p1);
        x += pChartDlg->kX;
    }
}

void ChartImage::DrawImageXY(Gdiplus::Graphics* g)
{
    Pen pen(color, width);
    pen.SetAlignment(PenAlignmentCenter);
    pen.SetEndCap(LineCapRoundAnchor);
    PointF p1;
    PointF p0 = PointF(pChartDlg->x0 + valuesX[0] * pChartDlg->kX, pChartDlg->y0 - valuesY[0] * pChartDlg->kY);;
    int amount = (valuesX.size() < valuesY.size()) ? valuesX.size() : valuesY.size();
    for (int i = 1; i < amount; i++)
    {
        p1 = PointF(pChartDlg->x0 + valuesX[i] * pChartDlg->kX, pChartDlg->y0 - valuesY[i] * pChartDlg->kY);
        g->DrawLine(&pen, p0, p1);
        p0 = PointF(p1);
    }
}
