#pragma once

#include <wx/wx.h>
#include "CRREMData.h"

struct CRREMData;

class CrremDiagram : public wxControl
{
  public:
    CrremDiagram(wxWindow* parent);

    void OnPaint(wxPaintEvent& event);

    void setCrremData(const CRREMData* pCrremData) { m_pCrremData = pCrremData; }

  private:
    void drawCoordAxis(wxGraphicsContext* pContext, const wxPoint2DDouble& origin, double xChartRange, double yChartRange, double arrowLength, double arrowWidth);
    void drawTicks(wxGraphicsContext* pContext, const wxPoint2DDouble& origin, double xChartRange, double yChartRange, double tickLength);
    void drawLegend(wxGraphicsContext* pContext, const wxPoint2DDouble& origin, double xChartRange, double tickLength);
    void drawBezier(wxGraphicsContext* pContext, const std::vector<wxPoint2DDouble>& points);
    static int NiceNumber(double value);
    static int yAxisStep(int minValue, int maxValue);

  private:
    const CRREMData* m_pCrremData = nullptr;
};