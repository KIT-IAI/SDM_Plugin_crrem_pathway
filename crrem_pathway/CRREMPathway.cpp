#include "CRREMData.h"
#include "CRREMPathway.h"
#include "CRREMPathwayDlgImpl.h"
#include "NaisConnector.h"
#include <LiveLogFeature.hpp>
#include <stdutils/utf8.hpp>
#include <vector>


CRREMPathway::CRREMPathway(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates, sdm::plugin::LiveLogInterface* pLogger, CRREMPathwayDlgImpl* pDlg)
  : m_pDB(pDB), m_pStates(pStates), m_pLogger(pLogger), m_pCRREMPathwayDlg(pDlg)
{
}

CRREMData CRREMPathway::calculate()
{
  if (!m_pCRREMPathwayDlg)
  {
    return {};
  }

  CRREMData crremData;

  m_pCRREMPathwayDlg->getDialogData(crremData);

  int buildingType = m_pCRREMPathwayDlg->getBuildingType();
  crremData.pathway = getPathway(crremData.country, crremData.buildingType, crremData.scenario, crremData.valueType, crremData.startYear, crremData.targetYear);
  crremData.updateStartValue();
  crremData.resultPathway = exponentialPath(crremData.startYear, crremData.targetYear, crremData.startValue, crremData.annualReductionRate);
  crremData.strandingYear = calculateStrandingYear(crremData.pathway, crremData.resultPathway, crremData.startYear);

  m_pCRREMPathwayDlg->setStartValue(crremData.startValue);

  return crremData;
}

std::vector<double> CRREMPathway::getPathway(const std::string& country, const std::string& buildingType, ScenarioType scenario, ValueType value, int startYear, int targetYear)
{
  NaisConector nais(m_pLogger);

  if (nais.connect())
  {
    if (scenario == CRREM_1_5)
    {
      return nais.getPathway(value, country, buildingType, startYear, targetYear);
    }
    else
    {
      assert(0);
    }
  }
  else
  {
    assert(0);
  }

  return {};
}

// Linearer Reduktionspfad
std::vector<double> CRREMPathway::linearPath(int startYear, int targetYear, double startValue, double targetValue)
{
  std::vector<double> path;

  int years = targetYear - startYear;

  for (int i = 0; i <= years; ++i)
  {
    double t = static_cast<double>(i) / years;
    double value = startValue + (targetValue - startValue) * t;
    path.push_back(value);
  }

  return std::move(path);
}

std::vector<double> CRREMPathway::exponentialPath(int startYear, int targetYear, double startValue, double annualReductionRate)
{
  std::vector<double> path;

  double value = startValue;

  for (int year = startYear; year <= targetYear; ++year)
  {
    path.push_back(value);
    value *= (1.0 - annualReductionRate / 100);
  }

  return std::move(path);
}

std::optional<int> CRREMPathway::calculateStrandingYear(const std::vector<double>& crremPath, const std::vector<double>& actualPath, int startYear)
{
  if (crremPath.empty() || actualPath.empty())
    return std::nullopt;

  int resultStartIndex(startYear - 2020);

  for (size_t i = 0; i < crremPath.size(); ++i)
  {
    if (i < resultStartIndex || i >= resultStartIndex + actualPath.size())
    {
      continue;
    }

    if (actualPath[i - resultStartIndex] > crremPath[i])
    {
      return startYear + static_cast<int>(i - resultStartIndex);
    }
  }

  return std::nullopt;
}







