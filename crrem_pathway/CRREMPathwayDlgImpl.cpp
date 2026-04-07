#include "CRREMPathwayDlgImpl.h"
#include "CRREMEnergyDlgImpl.h"
#include "NaisConnector.h"
#include "GoogleGeocode.h"
#include "DataAnalyzer.h"
#include <wx/valnum.h>
#include <stdutils/utf8.hpp>

IMPLEMENT_APP_NO_MAIN(MyApp)

bool MyApp::OnInit()
{
  return true;
}

int MyApp::OnExit()
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////

CRREMPathwayDlgImpl::CRREMPathwayDlgImpl(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
  : CRREMPathwayDlg(parent, id, title, pos, size, style)
{
  SetSize(wxSize(379, 1026));
  Centre();

  SetDoubleBuffered(true);

  wxSize minSize = this->GetBestSize();
  this->SetSize(minSize);
}

CRREMPathwayDlgImpl::~CRREMPathwayDlgImpl()
{
  m_pCRREMPathwayAction->setIsDialogOpen(false);
}

void CRREMPathwayDlgImpl::calculate(IfcDB::ifcOid oid)
{
  fillDialogData(oid);

  CRREMPathway pathway(m_pDB, m_pStates, m_pLogger, this);
  m_CRREMData = pathway.calculate();

  m_crremDiagram->setCrremData(&m_CRREMData);
}

int CRREMPathwayDlgImpl::getBuildingType()
{
  return m_buildingTypeChoice->GetSelection();
}

void CRREMPathwayDlgImpl::getDialogData(CRREMData& crremData)
{
  if (!m_buildingTypeChoice->IsEmpty() && m_buildingTypeChoice->GetSelection() != wxNOT_FOUND)
  {
    crremData.buildingType = m_buildingTypeChoice->GetStringSelection();
  }

  crremData.country = m_countryChoice->GetStringSelection();
  crremData.scenario = static_cast<ScenarioType>(m_scenarioChoice->GetSelection());
  crremData.valueType = static_cast<ValueType>(m_impactTypeChoice->GetSelection());
  crremData.totalArea = m_area;
  crremData.annualValue = m_annualValue;
  crremData.startYear = m_startYear;
  crremData.targetYear = m_targetYear;
  crremData.startValue = m_startValue;
  crremData.energyDemands = m_CRREMData.energyDemands;
  crremData.annualReductionRate = m_annualReductionRate;
}

void CRREMPathwayDlgImpl::setStartValue(double startValue)
{
  m_startValue = startValue;

  TransferDataToWindow();
}

void CRREMPathwayDlgImpl::OnInitDialog(wxInitDialogEvent& event)
{
  m_pCRREMPathwayAction->setIsDialogOpen(true);

  wxFloatingPointValidator<double> validatorArea(2, &m_area);
  validatorArea.SetRange(0.0, 1000000.0);
  m_areaCtrl->SetValidator(validatorArea);

  wxFloatingPointValidator<double> validatorEnergy(2, &m_annualValue);
  validatorEnergy.SetRange(0.0, 1000000.0);
  m_annualValueCtrl->SetValidator(validatorEnergy);

  wxIntegerValidator<int> validatorStartYear(&m_startYear);
  validatorStartYear.SetRange(2020, 2050);
  m_startYearCtrl->SetValidator(validatorStartYear);

  wxIntegerValidator<int> validatorTargetYear(&m_targetYear);
  validatorTargetYear.SetRange(2020, 2050);
  m_targetYearCtrl->SetValidator(validatorTargetYear);

  wxFloatingPointValidator<double> validatorStartValue(1, &m_startValue);
  validatorStartValue.SetRange(0.0, 1000.0);
  m_startValueCtrl->SetValidator(validatorStartValue);

  wxIntegerValidator<int> validatorAnnualReductionRate(&m_annualReductionRate);
  validatorAnnualReductionRate.SetRange(0, 20);
  m_annualReductionRateCtrl->SetValidator(validatorAnnualReductionRate);

  m_startYear = 2020;
  m_targetYear = 2050;
  m_annualReductionRate = 0;

  fillDialogData();
  fillNaisData();

  TransferDataToWindow();

  m_startYear = m_CRREMData.startYear;
  m_targetYear = m_CRREMData.targetYear;
  m_annualReductionRate = m_CRREMData.annualReductionRate;

  DoGetBestClientSize();
  DoGetBestSize();
}

void CRREMPathwayDlgImpl::OnAreaCheck(wxCommandEvent& event)
{
  if (m_areaOverrideCheckBox->GetValue())
  {
    m_areaCtrl->SetEditable(true);
    m_startValueCtrl->SetEditable(false);
  }
  else
  {
    m_areaCtrl->SetEditable(false);
    m_startValueCtrl->SetEditable(true);
  }
}

void CRREMPathwayDlgImpl::OnEnergyButtonClick(wxCommandEvent& event)
{
  CRREMEnergyDlgImpl dlg(nullptr, wxID_ANY, wxT("CRREM Energy Dialog"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP);
  dlg.setEnergyDemands(&m_CRREMData.energyDemands);

  if (dlg.ShowModal() == wxID_OK)
  {
    m_CRREMData.updateStartValue();

    setStartValue(m_CRREMData.startValue);
  }
}

void CRREMPathwayDlgImpl::OnEnergyCheck(wxCommandEvent& event)
{
  if (m_annualValueOverrideCheckBox->GetValue())
  {
    m_annualValueCtrl->SetEditable(true);
    m_startValueCtrl->SetEditable(false);
    m_energyButton->Enable(false);
  }
  else
  {
    m_annualValueCtrl->SetEditable(false);
    m_startValueCtrl->SetEditable(true);
    m_energyButton->Enable(true);
  }
}

void CRREMPathwayDlgImpl::OnImpactTypeChoice(wxCommandEvent& event)
{
  int pos = m_impactTypeChoice->GetSelection();

  std::stringstream labelStream;
  labelStream << "Start value [" << m_impactTypeChoice->GetString(pos) << "/m²]";

  m_startValueText->SetLabel(labelStream.str());

  if (pos == 0)
  {
    m_annualValueText->SetLabel("Annual energy demand [kWh]");
  }
  else if (pos == 1)
  {
    m_annualValueText->SetLabel("Annual emission [kgCO²]");
  }

  Refresh();
}

void CRREMPathwayDlgImpl::createCRREMPathway()
{
  std::ostringstream streamBuildig;
  streamBuildig << "Calculating CRREM pathway for building: " << toUtf8(m_nameCtrl->GetValue());
  m_pLogger->log(sdm::plugin::MessageLevel::Info, "CRREMPathway", streamBuildig.str());

  CRREMPathway pathway(m_pDB, m_pStates, m_pLogger, this);
  m_CRREMData = pathway.calculate();

  m_crremDiagram->setCrremData(&m_CRREMData);
  m_crremDiagram->Refresh();

  if (!m_annualValueOverrideCheckBox->GetValue())
  {
    m_CRREMData.updateAnnualValue();
  }
  
  m_annualValue = m_CRREMData.annualValue;

  TransferDataToWindow();
}

std::string CRREMPathwayDlgImpl::getCountryCode()
{
  // identify country code
  IfcDB::Point latLon = m_pDB->getGeoReference().getRefPointLatLon();
  GoogleGeocode geoCode(latLon);

  return toUtf8(geoCode.getCountryShortName());
}

void CRREMPathwayDlgImpl::clearDialogData()
{
  m_nameCtrl->SetValue("");
  m_yearOfConstructionCtrl->SetValue("");
  m_buildingTypeCtrl->SetValue("");

  m_area = 0.0;
  m_annualValue = 0.0;
  m_startValue = 0.0;

  m_startValueCtrl->SetEditable(true);

  TransferDataToWindow();
}

void CRREMPathwayDlgImpl::fillDialogData(IfcDB::ifcOid oid/*= 0*/)
{
  std::unique_ptr<DataAnalyzer> analyzer = nullptr;

  if (oid == 0)
  {
    analyzer = DataAnalyzerFactory::create(m_pDB, m_pStates);
  }
  else
  {
    analyzer = DataAnalyzerFactory::create(m_pDB, oid);
  }

  analyzer.get()->analyseBuildingParameter();
  auto buildingParameter = analyzer->getBuildingParameter();

  m_CRREMData.energyDemands = buildingParameter.energyDemands;

  m_nameCtrl->SetValue(wxString::FromUTF8(buildingParameter.buildingName));

  if (buildingParameter.yearOfConstruction > 0)
  {
    m_yearOfConstructionCtrl->SetValue(wxString::FromDouble(buildingParameter.yearOfConstruction));
  }

  m_buildingTypeCtrl->SetValue(buildingParameter.buildingType);

  if (!m_areaOverrideCheckBox->GetValue() && buildingParameter.totalArea > 0.0)
  {
    m_CRREMData.totalArea = buildingParameter.totalArea;
    m_area = buildingParameter.totalArea;
  }

  if (!m_annualValueOverrideCheckBox->GetValue() && buildingParameter.totalEnergyDemand > 0.0)
  {
    m_CRREMData.annualValue = buildingParameter.totalEnergyDemand;

    if (m_CRREMData.valueType == KWH)
    {
      m_annualValue = buildingParameter.totalEnergyDemand;
    }
    else if (m_CRREMData.valueType == CO2)
    {
      double totalCO2(0.0);

      for (const auto& energyData : m_CRREMData.energyDemands)
      {
        totalCO2 += energyData.energyDemand * energyData.emissionFactor;
      }

      m_annualValue = totalCO2;
    }
  }

  if (m_area > 0.0 && m_annualValue > 0.0)
  {
    m_CRREMData.startValue = m_annualValue / m_area;
    m_startValue = m_CRREMData.startValue;
  }

  if (m_area > 0.0 && m_annualValue > 0.0 &&
      (m_areaOverrideCheckBox->GetValue() || m_annualValueOverrideCheckBox->GetValue()))
  {
    m_startValueCtrl->SetEditable(false);
  }

  TransferDataToWindow();
}

void CRREMPathwayDlgImpl::fillNaisData()
{
  NaisConector nais(m_pLogger);

  if (nais.connect())
  {
    ValueType valueType(KWH);

    if (m_impactTypeChoice->GetSelection() == 0)
    {
      valueType = KWH;
    }
    else if (m_impactTypeChoice->GetSelection() == 1)
    {
      valueType = CO2;
    }
    else
    {
      assert(0);
    }

    std::string countryCode = getCountryCode();

    std::vector<std::string> countries = nais.getCountries(valueType);

    int pos(0);
    int selectionPos(0);
    for (const auto& country : countries)
    {
      m_countryChoice->AppendString(country);

      if (countryCode == country)
      {
        selectionPos = pos;
      }

      ++pos;
    }

    m_countryChoice->SetSelection(selectionPos);

    std::vector<std::string> assetLabels = nais.getAssetLabels(valueType, countryCode);

    for (const auto& asset : assetLabels)
    {
      m_buildingTypeChoice->AppendString(asset);
    }
  }
}

void CRREMPathwayDlgImpl::OnApply(wxCommandEvent& event)
{
  TransferDataFromWindow();

  if (m_buildingTypeChoice->GetSelection() == wxNOT_FOUND)
  {
    wxMessageBox("Please select a building type!", "Information", wxOK | wxICON_INFORMATION);

    return;
  }

  createCRREMPathway();

  CRREMPathwayDlg::OnApply(event);
}

void CRREMPathwayDlgImpl::OnCancel(wxCommandEvent& event)
{
  m_pCRREMPathwayAction->setIsDialogOpen(false);

  CRREMPathwayDlg::OnCancel(event);
}

void CRREMPathwayDlgImpl::OnClose(wxCloseEvent& event)
{
  m_pCRREMPathwayAction->setIsDialogOpen(false);

  Destroy();

  CRREMPathwayDlg::OnClose(event);
}

void CRREMPathwayDlgImpl::OnOK(wxCommandEvent& event)
{
  m_pCRREMPathwayAction->setIsDialogOpen(false);

  CRREMPathwayDlg::OnOK(event);
}
