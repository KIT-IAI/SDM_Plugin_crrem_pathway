#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "CRREMPathwayDlg.h"
#include "CRREMPathway.h"
#include "CRREMPathwayPlugin.h"
#include "CRREMData.h"
#include <LiveLogFeature.hpp>

///////////////////////////////////////////////////////////////////////////

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
    virtual int OnExit();
};

///////////////////////////////////////////////////////////////////////////

class CRREMPathwayDlgImpl : public CRREMPathwayDlg
{
  public:
    CRREMPathwayDlgImpl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(390, 977), long style = wxDEFAULT_DIALOG_STYLE);
    ~CRREMPathwayDlgImpl();

  public:
    void setCRREMPathwayAction(CRREMPathwayAction* pCRREMPathwayAction) { m_pCRREMPathwayAction = pCRREMPathwayAction; }
    void setLogger(sdm::plugin::LiveLogInterface* pLogger) { m_pLogger = pLogger; }
    void setDB(IfcDB::Populationi* pDB) { m_pDB = pDB; }
    void setStates(IfcDB::utils::PopulationStates* pSates) { m_pStates = pSates; }

    void calculate(IfcDB::ifcOid oid);
    int getBuildingType();
    void getDialogData(CRREMData& crremData);
    void setStartValue(double startValue);
    void clearDialogData();

  private:
    void createCRREMPathway();
    std::string getCountryCode();
    void fillDialogData(IfcDB::ifcOid oid = 0);
    void fillNaisData();

  private:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnInitDialog(wxInitDialogEvent& event);
    virtual void OnAreaCheck(wxCommandEvent& event);
    virtual void OnEnergyButtonClick(wxCommandEvent& event);
    virtual void OnEnergyCheck(wxCommandEvent& event);
    virtual void OnImpactTypeChoice(wxCommandEvent& event);
    virtual void OnApply(wxCommandEvent& event);
    virtual void OnCancel(wxCommandEvent& event);
    virtual void OnOK(wxCommandEvent& event);

  private:
    CRREMPathwayAction* m_pCRREMPathwayAction = nullptr;
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;
    sdm::plugin::LiveLogInterface* m_pLogger = nullptr;
    PluginObserver* m_pObserver = nullptr;
    CRREMData m_CRREMData;

    double m_area = 0.0;
    double m_annualValue = 0.0;
    int m_startYear = 0;
    int m_targetYear = 0;
    double m_startValue = 0.0;
    int m_annualReductionRate = 0;
};
