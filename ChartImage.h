#pragma once
#include "ChartDlg.h"

class ChartDlg;

class ChartImage
{
private:
    ChartDlg* pChartDlg;
    //Pen* imgPen;

public:
    ChartImage(ChartDlg *);
    ~ChartImage();

    Color color;
    double width, minX, maxX, minY, maxY, scale, shiftX, shiftY;
    std::vector<double> valuesX, valuesY;

    void AddValue(double);
    void AddValue(double, double);
    void ClearValues();
    void InitImage(Color, double);
    void DrawImage(Gdiplus::Graphics*);
    void DrawImageXY(Gdiplus::Graphics*);
};


